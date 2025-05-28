// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/Axes.hxx>
#include <EPHist/BinIndex.hxx>
#include <EPHist/BinIndexRange.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

#include <array>
#include <stdexcept>
#include <tuple>

TEST(Axes, MixedTypes) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);

  EPHist::Detail::Axes axes({regularAxis, variableBinAxis, regularAxis});
  EXPECT_EQ(axes.GetNumDimensions(), 3);
  const auto &v = axes.GetVector();
  ASSERT_EQ(v.size(), 3);
  EXPECT_EQ(v[0].index(), 0);
  EXPECT_EQ(v[1].index(), 1);
  EXPECT_EQ(v[2].index(), 0);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&v[0]) != nullptr);
  ASSERT_TRUE(std::get_if<EPHist::VariableBinAxis>(&v[1]) != nullptr);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&v[2]) != nullptr);

  std::vector<EPHist::AxisVariant> newAxes{variableBinAxis, regularAxis};
  axes = EPHist::Detail::Axes(newAxes);
  ASSERT_EQ(axes.GetNumDimensions(), 2);
  ASSERT_EQ(v.size(), 2);
}

TEST(Axes, ComputeBin) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  std::vector<double> binsY;
  for (std::size_t i = 0; i < BinsY; i++) {
    binsY.push_back(i);
  }
  binsY.push_back(BinsY);
  EPHist::VariableBinAxis axisY(binsY);
  EPHist::Detail::Axes axes({axisX, axisY});

  auto axisBin = axes.ComputeBin(std::make_tuple(1, 2));
  EXPECT_EQ(axisBin.first, (BinsY + 2) + 2);
  EXPECT_TRUE(axisBin.second);

  axisBin = axes.ComputeBin<EPHist::RegularAxis, EPHist::VariableBinAxis>(2, 3);
  EXPECT_EQ(axisBin.first, 2 * (BinsY + 2) + 3);
  EXPECT_TRUE(axisBin.second);

  std::array<EPHist::BinIndex, 2> args = {4, 5};
  axisBin = axes.ComputeBin(args);
  EXPECT_EQ(axisBin.first, 4 * (BinsY + 2) + 5);
  EXPECT_TRUE(axisBin.second);

  args[1] = EPHist::BinIndex();
  axisBin = axes.ComputeBin(args);
  EXPECT_FALSE(axisBin.second);
}

TEST(Axes, ComputeBinInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Detail::Axes axes1({axis});
  ASSERT_EQ(axes1.GetNumDimensions(), 1);
  EPHist::Detail::Axes axes2({axis, axis});
  ASSERT_EQ(axes2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(axes1.ComputeBin(std::make_tuple(1)));
  EXPECT_THROW(axes1.ComputeBin(std::make_tuple(1, 2)), std::invalid_argument);

  EXPECT_THROW(axes2.ComputeBin(std::make_tuple(1)), std::invalid_argument);
  EXPECT_NO_THROW(axes2.ComputeBin(std::make_tuple(1, 2)));
  EXPECT_THROW(axes2.ComputeBin(std::make_tuple(1, 2, 3)),
               std::invalid_argument);
}

TEST(Axes, TemplatedComputeBinInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Detail::Axes axes1({axis});
  ASSERT_EQ(axes1.GetNumDimensions(), 1);
  EPHist::Detail::Axes axes2({axis, axis});
  ASSERT_EQ(axes2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(axes1.ComputeBin<EPHist::RegularAxis>(1));
  EXPECT_THROW(
      (axes1.ComputeBin<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)),
      std::invalid_argument);

  EXPECT_THROW(axes2.ComputeBin<EPHist::RegularAxis>(1), std::invalid_argument);
  EXPECT_NO_THROW(
      (axes2.ComputeBin<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)));
  EXPECT_THROW((axes2.ComputeBin<EPHist::RegularAxis, EPHist::RegularAxis,
                                 EPHist::RegularAxis>(1, 2, 3)),
               std::invalid_argument);
}

TEST(Axes, TemplatedComputeInvalidAxis) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);
  EPHist::Detail::Axes axes1({variableBinAxis});
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  EPHist::Detail::Axes axes2({regularAxis, regularAxis});

  EXPECT_THROW((axes1.ComputeBin<EPHist::RegularAxis>(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(axes1.ComputeBin<EPHist::VariableBinAxis>(1));

  EXPECT_NO_THROW(
      (axes2.ComputeBin<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)));
  EXPECT_THROW(
      (axes2.ComputeBin<EPHist::RegularAxis, EPHist::VariableBinAxis>(1, 2)),
      std::invalid_argument);
  EXPECT_THROW(
      (axes2.ComputeBin<EPHist::VariableBinAxis, EPHist::RegularAxis>(1, 2)),
      std::invalid_argument);
  EXPECT_THROW(
      (axes2.ComputeBin<EPHist::VariableBinAxis, EPHist::VariableBinAxis>(1,
                                                                          2)),
      std::invalid_argument);
}

TEST(Axes, Slice) {
  static constexpr std::size_t Bins = 20;
  const EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  const EPHist::VariableBinAxis variableBinAxis(bins);

  const EPHist::Detail::Axes axes({regularAxis, variableBinAxis});
  std::array<EPHist::BinIndexRange, 2> ranges{EPHist::BinIndexRange(2, 12),
                                              EPHist::BinIndexRange(8, 15)};
  const auto sliced = axes.Slice(ranges);
  ASSERT_EQ(sliced.size(), 2);
  {
    const auto *slicedAxis = std::get_if<EPHist::RegularAxis>(&sliced[0]);
    ASSERT_TRUE(slicedAxis != nullptr);
    EXPECT_TRUE(slicedAxis->AreFlowBinsEnabled());
    EXPECT_EQ(slicedAxis->GetNumBins(), 10);
    EXPECT_FLOAT_EQ(slicedAxis->GetLow(), 2);
    EXPECT_FLOAT_EQ(slicedAxis->GetHigh(), 12);
  }
  {
    const auto *slicedAxis = std::get_if<EPHist::VariableBinAxis>(&sliced[1]);
    ASSERT_TRUE(slicedAxis != nullptr);
    EXPECT_TRUE(slicedAxis->AreFlowBinsEnabled());
    ASSERT_EQ(slicedAxis->GetNumBins(), 7);
    EXPECT_FLOAT_EQ(slicedAxis->GetBinEdge(0), 8);
    EXPECT_FLOAT_EQ(slicedAxis->GetBinEdge(7), 15);
  }
}

TEST(Axes, SliceInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Detail::Axes axes1({axis});
  ASSERT_EQ(axes1.GetNumDimensions(), 1);
  EPHist::Detail::Axes axes2({axis, axis});
  ASSERT_EQ(axes2.GetNumDimensions(), 2);

  EPHist::BinIndexRange range(5, 10);
  std::array<EPHist::BinIndexRange, 1> ranges1{range};
  std::array<EPHist::BinIndexRange, 2> ranges2{range, range};
  std::array<EPHist::BinIndexRange, 3> ranges3{range, range, range};

  EXPECT_NO_THROW(axes1.Slice(ranges1));
  EXPECT_THROW(axes1.Slice(ranges2), std::invalid_argument);

  EXPECT_THROW(axes2.Slice(ranges1), std::invalid_argument);
  EXPECT_NO_THROW(axes2.Slice(ranges2));
  EXPECT_THROW(axes2.Slice(ranges3), std::invalid_argument);
}
