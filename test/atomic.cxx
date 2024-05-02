// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>

#include <gtest/gtest.h>

TEST(IntRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.FillAtomic(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }
  h1.FillAtomic(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillAtomicTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.FillAtomic(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i));
  }
  h1.FillAtomic(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, TemplatedFillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.FillAtomic<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i);
  }
  h1.FillAtomic<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(LongLongRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<long long> h1(Bins, 0, Bins);

  h1.FillAtomic(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }
  h1.FillAtomic(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(LongLongRegular1D, FillAtomicTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<long long> h1(Bins, 0, Bins);

  h1.FillAtomic(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i));
  }
  h1.FillAtomic(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(LongLongRegular1D, TemplatedFillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<long long> h1(Bins, 0, Bins);

  h1.FillAtomic<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i);
  }
  h1.FillAtomic<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(FloatRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  h1.FillAtomic(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }
  h1.FillAtomic(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(FloatRegular1D, FillAtomicTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  h1.FillAtomic(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i));
  }
  h1.FillAtomic(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(FloatRegular1D, TemplatedFillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  h1.FillAtomic<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i);
  }
  h1.FillAtomic<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(FloatRegular1D, FillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  h1.FillAtomic(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }
  h1.FillAtomic(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(DoubleRegular1D, FillAtomicTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  h1.FillAtomic(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i));
  }
  h1.FillAtomic(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(DoubleRegular1D, TemplatedFillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  h1.FillAtomic<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i);
  }
  h1.FillAtomic<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(DoubleRegular1D, FillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomicTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, TemplatedFillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}
