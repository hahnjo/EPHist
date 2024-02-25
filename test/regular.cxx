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
  EXPECT_EQ(h1.GetNumDimensions(), 1);

  EPHist::RegularAxis axis(2 * Bins, 0, Bins);
  h1 = EPHist::EPHist<int>(axis);
  EXPECT_EQ(h1.GetNumBins(), 2 * Bins);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
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

TEST(IntRegular2D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h2({axis, axis});
  EXPECT_EQ(h2.GetNumBins(), Bins * Bins);
  EXPECT_EQ(h2.GetNumDimensions(), 2);

  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  h2 = EPHist::EPHist<int>({axis, axisY});
  EXPECT_EQ(h2.GetNumBins(), Bins * BinsY);
  EXPECT_EQ(h2.GetNumDimensions(), 2);
}

TEST(IntRegular2D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill(x, y);
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY; i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, Constructor) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});
  EXPECT_EQ(h3.GetNumBins(), BinsX * BinsY * BinsZ);
  EXPECT_EQ(h3.GetNumDimensions(), 3);
}

TEST(IntRegular3D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      for (std::size_t z = 0; z < BinsZ; z++) {
        h3.Fill(x, y, z);
      }
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY * BinsZ; i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}
