// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/Axes.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

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
