// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/CategoricalAxis.hxx>
#include <EPHist/EPHist.hxx>

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

TEST(CategoricalAxis, Constructor) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EXPECT_EQ(axis.GetNumBins(), 3);
  EXPECT_EQ(axis.GetTotalNumBins(), 4);

  axis = EPHist::CategoricalAxis(categories, /*enableOverflowBin=*/false);
  EXPECT_EQ(axis.GetNumBins(), 3);
  EXPECT_EQ(axis.GetTotalNumBins(), 3);

  std::vector<std::string> duplicate = {"a", "a"};
  EXPECT_THROW(EPHist::CategoricalAxis d(duplicate), std::invalid_argument);
}

TEST(CategoricalAxis, Equality) {
  std::vector<std::string> categoriesA = {"a", "b", "c"};
  std::vector<std::string> categoriesB = {"c", "b", "a"};
  std::vector<std::string> categoriesC = {"a", "ab", "abc"};

  EPHist::CategoricalAxis axisA(categoriesA);
  EPHist::CategoricalAxis axisANoOverflow(categoriesA,
                                          /*enableOverflowBin=*/false);
  EPHist::CategoricalAxis axisA2(categoriesA);
  EPHist::CategoricalAxis axisB(categoriesB);
  EPHist::CategoricalAxis axisC(categoriesC);

  EXPECT_TRUE(axisA == axisA);
  EXPECT_TRUE(axisA == axisA2);

  EXPECT_FALSE(axisA == axisANoOverflow);

  EXPECT_FALSE(axisA == axisB);
  EXPECT_FALSE(axisA == axisC);
  EXPECT_FALSE(axisB == axisC);
}

TEST(CategoricalAxis, GetBin) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);

  {
    auto underflow = EPHist::BinIndex::Underflow();
    auto axisBin = axis.GetBin(underflow);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoOverflow.GetBin(underflow);
    EXPECT_FALSE(axisBin.second);
  }

  for (std::size_t i = 0; i < categories.size(); i++) {
    auto axisBin = axis.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoOverflow.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Out of bounds
  {
    auto axisBin = axis.GetBin(categories.size());
    EXPECT_EQ(axisBin.first, categories.size());
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoOverflow.GetBin(categories.size());
    EXPECT_EQ(axisBin.first, categories.size());
    EXPECT_FALSE(axisBin.second);
  }

  {
    auto overflow = EPHist::BinIndex::Overflow();
    auto axisBin = axis.GetBin(overflow);
    EXPECT_TRUE(axisBin.second);
    EXPECT_EQ(axisBin.first, categories.size());
    axisBin = axisNoOverflow.GetBin(overflow);
    EXPECT_EQ(axisBin.first, categories.size());
    EXPECT_FALSE(axisBin.second);
  }

  {
    EPHist::BinIndex invalid;
    auto axisBin = axis.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoOverflow.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(CategoricalAxis, ComputeBin) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);

  for (std::size_t i = 0; i < categories.size(); i++) {
    auto axisBin = axis.ComputeBin(categories[i]);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoOverflow.ComputeBin(categories[i]);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Overflow
  for (std::string overflow : {"", "d"}) {
    auto axisBin = axis.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, 3);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoOverflow.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, 3);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(CategoricalAxis, Slice) {
  std::vector<std::string> categories = {"a", "b", "c", "d"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);

  const auto full = EPHist::BinIndexRange::Full(categories.size());
  const auto full0 = EPHist::BinIndexRange::Full(0);
  for (auto &&a : {axis, axisNoOverflow}) {
    for (auto &&f : {full, full0}) {
      const auto slice = a.Slice(f);
      EXPECT_TRUE(slice.IsOverflowBinEnabled());
      ASSERT_EQ(slice.GetNumBins(), 4);
      EXPECT_EQ(slice.GetTotalNumBins(), 5);
      EXPECT_EQ(slice.GetCategory(0), "a");
      EXPECT_EQ(slice.GetCategory(3), "d");
    }
  }

  const auto bin0 = EPHist::BinIndex(0);
  const auto binLast = EPHist::BinIndex(categories.size());
  const auto inner = EPHist::BinIndexRange(bin0, binLast);
  for (auto &&a : {axis, axisNoOverflow}) {
    const auto slice = a.Slice(inner);
    EXPECT_TRUE(slice.IsOverflowBinEnabled());
    ASSERT_EQ(slice.GetNumBins(), 4);
    EXPECT_EQ(slice.GetTotalNumBins(), 5);
    EXPECT_EQ(slice.GetCategory(0), "a");
    EXPECT_EQ(slice.GetCategory(3), "d");
  }

  const auto binB = EPHist::BinIndex(1);
  const auto binD = EPHist::BinIndex(3);
  const auto range = EPHist::BinIndexRange(binB, binD);
  for (auto &&a : {axis, axisNoOverflow}) {
    const auto slice = a.Slice(range);
    EXPECT_TRUE(slice.IsOverflowBinEnabled());
    ASSERT_EQ(slice.GetNumBins(), 2);
    EXPECT_EQ(slice.GetTotalNumBins(), 3);
    EXPECT_EQ(slice.GetCategory(0), "b");
    EXPECT_EQ(slice.GetCategory(1), "c");
  }
}

TEST(IntCategorical1D, Constructor) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);
  EXPECT_EQ(h1.GetTotalNumBins(), 4);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  const auto &axes = h1.GetAxes();
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 2);
  ASSERT_TRUE(std::get_if<EPHist::CategoricalAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::CategoricalAxis>(axes[0]).GetNumBins(), 3);
}

TEST(IntCategorical1D, Fill) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(categories[i]);
  }
  h1.Fill("d");

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, FillDiscard) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories, /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(categories[i]);
  }
  h1.Fill("d");

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, FillOnlyInner) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1NoOverflow(axisNoOverflow);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(categories[i]);
    h1NoOverflow.Fill(categories[i]);
  }

  for (std::size_t i = 0; i < categories.size(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoOverflow.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(categories.size()), 0);
}

TEST(IntCategorical1D, FillTuple) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(std::make_tuple(categories[i]));
  }
  h1.Fill(std::make_tuple("d"));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, FillTupleDiscard) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories, /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(std::make_tuple(categories[i]));
  }
  h1.Fill(std::make_tuple("d"));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, FillTupleOnlyInner) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1NoOverflow(axisNoOverflow);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill(std::make_tuple(categories[i]));
    h1NoOverflow.Fill(std::make_tuple(categories[i]));
  }

  for (std::size_t i = 0; i < categories.size(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoOverflow.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(categories.size()), 0);
}

TEST(IntCategorical1D, TemplatedFill) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill<EPHist::CategoricalAxis>(categories[i]);
  }
  h1.Fill<EPHist::CategoricalAxis>("d");

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, TemplatedFillDiscard) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories, /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1(axis);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill<EPHist::CategoricalAxis>(categories[i]);
  }
  h1.Fill<EPHist::CategoricalAxis>("d");

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntCategorical1D, TemplatedFillOnlyInner) {
  std::vector<std::string> categories = {"a", "b", "c"};

  EPHist::CategoricalAxis axis(categories);
  EPHist::EPHist<int> h1(axis);
  EPHist::CategoricalAxis axisNoOverflow(categories,
                                         /*enableOverflowBin=*/false);
  EPHist::EPHist<int> h1NoOverflow(axisNoOverflow);

  for (std::size_t i = 0; i < categories.size(); i++) {
    h1.Fill<EPHist::CategoricalAxis>(categories[i]);
    h1NoOverflow.Fill<EPHist::CategoricalAxis>(categories[i]);
  }

  for (std::size_t i = 0; i < categories.size(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoOverflow.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(categories.size()), 0);
}
