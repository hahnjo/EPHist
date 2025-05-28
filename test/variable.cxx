// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

#include <limits>
#include <tuple>

TEST(VariableBinAxis, Constructor) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetTotalNumBins(), Bins + 2);

  axis = EPHist::VariableBinAxis(bins, /*enableFlowBins=*/false);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetTotalNumBins(), Bins);
}

TEST(VariableBinAxis, Equality) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> binsA;
  for (std::size_t i = 0; i < Bins; i++) {
    binsA.push_back(i);
  }
  binsA.push_back(Bins);

  std::vector<double> binsB;
  for (std::size_t i = 0; i < Bins / 2; i++) {
    binsB.push_back(i);
  }
  binsB.push_back(Bins / 2);

  std::vector<double> binsC;
  for (std::size_t i = Bins / 2; i < Bins; i++) {
    binsC.push_back(i);
  }
  binsC.push_back(Bins);

  EPHist::VariableBinAxis axisA(binsA);
  EPHist::VariableBinAxis axisANoFlowBins(binsA, /*enableFlowBins=*/false);
  EPHist::VariableBinAxis axisA2(binsA);
  EPHist::VariableBinAxis axisB(binsB);
  EPHist::VariableBinAxis axisC(binsC);

  EXPECT_TRUE(axisA == axisA);
  EXPECT_TRUE(axisA == axisA2);

  EXPECT_FALSE(axisA == axisANoFlowBins);

  EXPECT_FALSE(axisA == axisB);
  EXPECT_FALSE(axisA == axisC);
  EXPECT_FALSE(axisB == axisC);
}

TEST(VariableBinAxis, GetBin) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);

  {
    auto underflow = EPHist::BinIndex::Underflow();
    auto axisBin = axis.GetBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto axisBin = axis.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Out of bounds
  {
    auto axisBin = axis.GetBin(Bins);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(Bins);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  {
    auto overflow = EPHist::BinIndex::Overflow();
    auto axisBin = axis.GetBin(overflow);
    EXPECT_TRUE(axisBin.second);
    EXPECT_EQ(axisBin.first, Bins + 1);
    axisBin = axisNoFlowBins.GetBin(overflow);
    EXPECT_FALSE(axisBin.second);
  }

  {
    EPHist::BinIndex invalid;
    auto axisBin = axis.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(VariableBinAxis, ComputeBin) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);

  // Underflow
  static constexpr double NegativeInfinity =
      -std::numeric_limits<double>::infinity();
  static constexpr double UnderflowLarge = -static_cast<double>(Bins);
  static constexpr double UnderflowSmall = -0.1;
  for (double underflow : {NegativeInfinity, UnderflowLarge, UnderflowSmall}) {
    auto axisBin = axis.ComputeBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto axisBin = axis.ComputeBin(i + 0.5);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(i + 0.5);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Overflow
  static constexpr double PositiveInfinity =
      std::numeric_limits<double>::infinity();
  static constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
  static constexpr double OverflowLarge = static_cast<double>(Bins * 2);
  static constexpr double OverflowSmall = Bins + 0.1;
  for (double overflow :
       {PositiveInfinity, NaN, OverflowLarge, OverflowSmall}) {
    auto axisBin = axis.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, Bins + 1);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, Bins + 1);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(VariableBinAxis, Slice) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);

  const auto full = EPHist::BinIndexRange::Full(Bins);
  const auto full0 = EPHist::BinIndexRange::Full(0);
  for (auto &&a : {axis, axisNoFlowBins}) {
    for (auto &&f : {full, full0}) {
      const auto slice = a.Slice(f);
      EXPECT_TRUE(slice.AreFlowBinsEnabled());
      ASSERT_EQ(slice.GetNumBins(), Bins);
      EXPECT_EQ(slice.GetTotalNumBins(), Bins + 2);
      EXPECT_FLOAT_EQ(slice.GetBinEdge(0), 0);
      EXPECT_FLOAT_EQ(slice.GetBinEdge(Bins), Bins);
    }
  }

  const auto bin0 = EPHist::BinIndex(0);
  const auto bin20 = EPHist::BinIndex(Bins);
  const auto inner = EPHist::BinIndexRange(bin0, bin20);
  for (auto &&a : {axis, axisNoFlowBins}) {
    const auto slice = a.Slice(inner);
    EXPECT_TRUE(slice.AreFlowBinsEnabled());
    ASSERT_EQ(slice.GetNumBins(), Bins);
    EXPECT_EQ(slice.GetTotalNumBins(), Bins + 2);
    EXPECT_FLOAT_EQ(slice.GetBinEdge(0), 0);
    EXPECT_FLOAT_EQ(slice.GetBinEdge(Bins), Bins);
  }

  const auto bin5 = EPHist::BinIndex(5);
  const auto bin15 = EPHist::BinIndex(15);
  const auto range = EPHist::BinIndexRange(bin5, bin15);
  for (auto &&a : {axis, axisNoFlowBins}) {
    const auto slice = a.Slice(range);
    EXPECT_TRUE(slice.AreFlowBinsEnabled());
    ASSERT_EQ(slice.GetNumBins(), 10);
    EXPECT_EQ(slice.GetTotalNumBins(), 12);
    EXPECT_FLOAT_EQ(slice.GetBinEdge(0), 5);
    EXPECT_FLOAT_EQ(slice.GetBinEdge(10), 15);
  }
}

TEST(IntVariableBin1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);
  EXPECT_EQ(h1.GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  const auto &axes = h1.GetAxes();
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 1);
  ASSERT_TRUE(std::get_if<EPHist::VariableBinAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::VariableBinAxis>(axes[0]).GetNumBins(), Bins);
}

TEST(IntVariableBin1D, Fill) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }
  h1.Fill(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, FillDiscard) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }
  h1.Fill(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, FillOnlyInner) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
    h1NoFlowBins.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}

TEST(IntVariableBin1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }
  h1.Fill(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, FillTupleDiscard) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }
  h1.Fill(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, FillTupleOnlyInner) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
    h1NoFlowBins.Fill(std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}

TEST(IntVariableBin1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);

  h1.Fill<EPHist::VariableBinAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::VariableBinAxis>(i);
  }
  h1.Fill<EPHist::VariableBinAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, TemplatedFillDiscard) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill<EPHist::VariableBinAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::VariableBinAxis>(i);
  }
  h1.Fill<EPHist::VariableBinAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntVariableBin1D, TemplatedFillOnlyInner) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::VariableBinAxis>(i);
    h1NoFlowBins.Fill<EPHist::VariableBinAxis>(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}
