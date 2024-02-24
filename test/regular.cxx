// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>

#include <gtest/gtest.h>

TEST(IntRegular1D, FillInt) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EXPECT_EQ(h1.GetNumBins(), Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}
