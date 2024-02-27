// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

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
}

TEST(VariableBinAxis, ComputeBin) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(axis.ComputeBin(i + 0.5), i);
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
  EXPECT_EQ(h1.GetNumBins(), Bins);
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

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
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

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
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

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::VariableBinAxis>(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}
