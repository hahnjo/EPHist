// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>

#include <gtest/gtest.h>

TEST(RegularAxis, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetLow(), 0);
  EXPECT_EQ(axis.GetHigh(), Bins);
}

TEST(RegularAxis, ComputeBin) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(axis.ComputeBin(i + 0.5), i);
  }
}

TEST(IntRegular1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EXPECT_EQ(h1.GetNumBins(), Bins);

  EPHist::RegularAxis axis(2 * Bins, 0, Bins);
  h1 = EPHist::EPHist<int>(axis);
  EXPECT_EQ(h1.GetNumBins(), 2 * Bins);
}

TEST(IntRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}
