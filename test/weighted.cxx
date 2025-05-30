// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/DoubleBinWithError.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/Weight.hxx>

#include <gtest/gtest.h>

#include <tuple>

TEST(FloatRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleBinWithErrorRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, Add) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> hA(Bins, 0, Bins);
  EPHist::EPHist<EPHist::DoubleBinWithError> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i, EPHist::Weight(0.5 + i * 0.1));
    hB.Fill(i, EPHist::Weight(1.5 + i * 0.2));
  }

  EPHist::EPHist<EPHist::DoubleBinWithError> hC(Bins, 0, Bins);
  hC.Add(hA);
  hC.Add(hB);

  hA.Add(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    double weightA = 0.5 + i * 0.1;
    double weightB = 1.5 + i * 0.2;
    double weightAB2 = weightA * weightA + weightB * weightB;
    EPHist::DoubleBinWithError binWithError = hA.GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, weightA + weightB);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weightAB2);

    binWithError = hB.GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, weightB);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weightB * weightB);

    binWithError = hC.GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, weightA + weightB);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weightAB2);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}
