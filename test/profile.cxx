// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/Profile.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/Weight.hxx>

#include <gtest/gtest.h>

#include <stdexcept>
#include <tuple>

TEST(Profile, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});
  EPHist::Profile<false> withoutError({axis});
}

TEST(ProfileWithError1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(i, 2 * i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
    EXPECT_FLOAT_EQ(binContent.fSum2, 1);
  }
}

TEST(ProfileWithError1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(std::make_tuple(i, 2 * i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
    EXPECT_FLOAT_EQ(binContent.fSum2, 1);
  }
}

TEST(ProfileWithError1D, FillTupleValue) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(std::make_tuple(i), 2 * i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
    EXPECT_FLOAT_EQ(binContent.fSum2, 1);
  }
}

TEST(ProfileWithError1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(i, 2 * i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
    EXPECT_FLOAT_EQ(binContent.fSum2, weight * weight);
  }
}

TEST(ProfileWithError1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(std::make_tuple(i, 2 * i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
    EXPECT_FLOAT_EQ(binContent.fSum2, weight * weight);
  }
}

TEST(ProfileWithError1D, FillTupleValueWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> withError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withError.Fill(std::make_tuple(i), 2 * i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
    EXPECT_FLOAT_EQ(binContent.fSum2, weight * weight);
  }
}

TEST(ProfileWithoutError1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(i, 2 * i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
  }
}

TEST(ProfileWithoutError1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(std::make_tuple(i, 2 * i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
  }
}

TEST(ProfileWithoutError1D, FillTupleValue) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(std::make_tuple(i), 2 * i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    EXPECT_FLOAT_EQ(binContent.fSumValues, 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, 1);
  }
}

TEST(ProfileWithoutError1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(i, 2 * i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
  }
}

TEST(ProfileWithoutError1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(std::make_tuple(i, 2 * i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
  }
}

TEST(ProfileWithoutError1D, FillTupleValueWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<false> withoutError({axis});

  for (std::size_t i = 0; i < Bins; i++) {
    withoutError.Fill(std::make_tuple(i), 2 * i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &&binContent = withoutError.GetBinContentAt(EPHist::BinIndex(i));
    const double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binContent.fSumValues, weight * 2 * i);
    EXPECT_FLOAT_EQ(binContent.fSumValues2, weight * 4 * i * i);
    EXPECT_FLOAT_EQ(binContent.fSum, weight);
  }
}
