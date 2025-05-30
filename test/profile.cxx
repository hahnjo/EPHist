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

TEST(Profile, FillInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  ASSERT_EQ(p1.GetNumDimensions(), 1);
  EPHist::Profile<true> p2({axis, axis});
  ASSERT_EQ(p2.GetNumDimensions(), 2);

  EXPECT_THROW(p1.Fill(1), std::invalid_argument);
  EXPECT_NO_THROW(p1.Fill(1, 2));
  EXPECT_THROW(p1.Fill(1, 2, 3), std::invalid_argument);

  EXPECT_THROW(p2.Fill(1, 2), std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(1, 2, 3));
  EXPECT_THROW(p2.Fill(1, 2, 3, 4), std::invalid_argument);
}

TEST(Profile, FillTupleInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  ASSERT_EQ(p1.GetNumDimensions(), 1);
  EPHist::Profile<true> p2({axis, axis});
  ASSERT_EQ(p2.GetNumDimensions(), 2);

  EXPECT_THROW(p1.Fill(std::make_tuple(1)), std::invalid_argument);
  EXPECT_NO_THROW(p1.Fill(std::make_tuple(1, 2)));
  EXPECT_THROW(p1.Fill(std::make_tuple(1, 2, 3)), std::invalid_argument);

  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2)), std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(std::make_tuple(1, 2, 3)));
  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2, 3, 4)), std::invalid_argument);
}

TEST(Profile, FillTupleValueInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  ASSERT_EQ(p1.GetNumDimensions(), 1);
  EPHist::Profile<true> p2({axis, axis});
  ASSERT_EQ(p2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(p1.Fill(std::make_tuple(1), 2));
  EXPECT_THROW(p1.Fill(std::make_tuple(1, 2), 3), std::invalid_argument);

  EXPECT_THROW(p2.Fill(std::make_tuple(1), 2), std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(std::make_tuple(1, 2), 3));
  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2, 3), 4), std::invalid_argument);
}

TEST(Profile, FillWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  EPHist::Profile<true> p2({axis, axis});

  EXPECT_THROW(p1.Fill(1, EPHist::Weight(1)), std::invalid_argument);
  EXPECT_NO_THROW(p1.Fill(1, 2, EPHist::Weight(1)));
  EXPECT_THROW(p1.Fill(1, 2, 3, EPHist::Weight(1)), std::invalid_argument);

  EXPECT_THROW(p2.Fill(1, 2, EPHist::Weight(1)), std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(1, 2, 3, EPHist::Weight(1)));
  EXPECT_THROW(p2.Fill(1, 2, 3, 4, EPHist::Weight(1)), std::invalid_argument);
}

TEST(Profile, FillTupleWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  EPHist::Profile<true> p2({axis, axis});

  EXPECT_THROW(p1.Fill(std::make_tuple(1), EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(p1.Fill(std::make_tuple(1, 2), EPHist::Weight(1)));
  EXPECT_THROW(p1.Fill(std::make_tuple(1, 2, 3), EPHist::Weight(1)),
               std::invalid_argument);

  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2), EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(std::make_tuple(1, 2, 3), EPHist::Weight(1)));
  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2, 3, 4), EPHist::Weight(1)),
               std::invalid_argument);
}

TEST(Profile, FillTupleValueWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::Profile<true> p1({axis});
  EPHist::Profile<true> p2({axis, axis});

  EXPECT_NO_THROW(p1.Fill(std::make_tuple(1), 2, EPHist::Weight(1)));
  EXPECT_THROW(p1.Fill(std::make_tuple(1, 2), 3, EPHist::Weight(1)),
               std::invalid_argument);

  EXPECT_THROW(p2.Fill(std::make_tuple(1), 2, EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(p2.Fill(std::make_tuple(1, 2), 3, EPHist::Weight(1)));
  EXPECT_THROW(p2.Fill(std::make_tuple(1, 2, 3), 4, EPHist::Weight(1)),
               std::invalid_argument);
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
