// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>

#include <gtest/gtest.h>

TEST(Basic, Clear) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h.Fill(i);
  }

  h.Clear();

  for (std::size_t i = 0; i < h.GetNumBins(); i++) {
    EXPECT_EQ(h.GetBinContent(i), 0);
  }
}
