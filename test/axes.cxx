// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/Axes.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

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
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::Detail::Axes axes({axisX, axisY});

  auto axisBin = axes.ComputeBin(std::make_tuple(1, 2));
  EXPECT_EQ(axisBin.first, (BinsY + 2) + 2);
  EXPECT_TRUE(axisBin.second);

  axisBin = axes.ComputeBin<EPHist::RegularAxis, EPHist::RegularAxis>(2, 3);
  EXPECT_EQ(axisBin.first, 2 * (BinsY + 2) + 3);
  EXPECT_TRUE(axisBin.second);
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
