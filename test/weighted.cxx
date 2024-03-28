// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/DoubleBinWithError.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/Weight.hxx>

#include <gtest/gtest.h>

#include <tuple>

TEST(DoubleRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleBinWithErrorRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}
