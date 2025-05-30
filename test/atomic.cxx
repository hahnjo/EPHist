// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/DoubleBinWithError.hxx>
#include <EPHist/EPHist.hxx>

#include <gtest/gtest.h>

TEST(Atomic, AddAtomicDifferentDimensions) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_THROW(h1.AddAtomic(h2), std::invalid_argument);
}

TEST(Atomic, AddAtomicDifferentAxisTypes) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  EPHist::EPHist<int> hA(regularAxis);
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);
  EPHist::EPHist<int> hB(variableBinAxis);

  EXPECT_THROW(hA.AddAtomic(hB), std::invalid_argument);
}

TEST(Atomic, AddAtomicUnequalRegularAxis) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> hA(Bins, 0, Bins);
  EPHist::EPHist<int> hB(Bins / 2, 0, Bins);

  EXPECT_THROW(hA.AddAtomic(hB), std::invalid_argument);
}

TEST(Atomic, FillAtomicInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.FillAtomic(1));
  EXPECT_THROW(h1.FillAtomic(1, 2), std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic(1), std::invalid_argument);
  EXPECT_NO_THROW(h2.FillAtomic(1, 2));
  EXPECT_THROW(h2.FillAtomic(1, 2, 3), std::invalid_argument);
}

TEST(Atomic, FillAtomicTupleInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.FillAtomic(std::make_tuple(1)));
  EXPECT_THROW(h1.FillAtomic(std::make_tuple(1, 2)), std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic(std::make_tuple(1)), std::invalid_argument);
  EXPECT_NO_THROW(h2.FillAtomic(std::make_tuple(1, 2)));
  EXPECT_THROW(h2.FillAtomic(std::make_tuple(1, 2, 3)), std::invalid_argument);
}

TEST(Atomic, TemplatedFillAtomicInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.FillAtomic<EPHist::RegularAxis>(1));
  EXPECT_THROW((h1.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)),
               std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic<EPHist::RegularAxis>(1), std::invalid_argument);
  EXPECT_NO_THROW(
      (h2.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)));
  EXPECT_THROW((h2.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis,
                              EPHist::RegularAxis>(1, 2, 3)),
               std::invalid_argument);
}

TEST(Atomic, FillAtomicWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.FillAtomic(1, EPHist::Weight(1)));
  EXPECT_THROW(h1.FillAtomic(1, 2, EPHist::Weight(1)), std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic(1, EPHist::Weight(1)), std::invalid_argument);
  EXPECT_NO_THROW(h2.FillAtomic(1, 2, EPHist::Weight(1)));
  EXPECT_THROW(h2.FillAtomic(1, 2, 3, EPHist::Weight(1)),
               std::invalid_argument);
}

TEST(Atomic, FillAtomicTupleWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.FillAtomic(std::make_tuple(1), EPHist::Weight(1)));
  EXPECT_THROW(h1.FillAtomic(std::make_tuple(1, 2), EPHist::Weight(1)),
               std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic(std::make_tuple(1), EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(h2.FillAtomic(std::make_tuple(1, 2), EPHist::Weight(1)));
  EXPECT_THROW(h2.FillAtomic(std::make_tuple(1, 2, 3), EPHist::Weight(1)),
               std::invalid_argument);
}

TEST(Atomic, TemplatedFillAtomicWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.FillAtomic<EPHist::RegularAxis>(1, EPHist::Weight(1)));
  EXPECT_THROW((h1.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis>(
                   1, 2, EPHist::Weight(1))),
               std::invalid_argument);

  EXPECT_THROW(h2.FillAtomic<EPHist::RegularAxis>(1, EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW((h2.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis>(
      1, 2, EPHist::Weight(1))));
  EXPECT_THROW((h2.FillAtomic<EPHist::RegularAxis, EPHist::RegularAxis,
                              EPHist::RegularAxis>(1, 2, 3, EPHist::Weight(1))),
               std::invalid_argument);
}

TEST(IntRegular1D, AddAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> hA(Bins, 0, Bins);
  EPHist::EPHist<int> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
    hB.Fill(i);
  }

  EPHist::EPHist<int> hC(Bins, 0, Bins);
  hC.AddAtomic(hA);
  hC.AddAtomic(hB);

  hA.AddAtomic(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(hA.GetBinContent(i), 2);
    EXPECT_EQ(hB.GetBinContent(i), 1);
    EXPECT_EQ(hC.GetBinContent(i), 2);
  }
}

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

TEST(LongLongRegular1D, AddAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<long long> hA(Bins, 0, Bins);
  EPHist::EPHist<long long> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
    hB.Fill(i);
  }

  EPHist::EPHist<long long> hC(Bins, 0, Bins);
  hC.AddAtomic(hA);
  hC.AddAtomic(hB);

  hA.AddAtomic(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(hA.GetBinContent(i), 2);
    EXPECT_EQ(hB.GetBinContent(i), 1);
    EXPECT_EQ(hC.GetBinContent(i), 2);
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

TEST(FloatRegular1D, AddAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> hA(Bins, 0, Bins);
  EPHist::EPHist<float> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
    hB.Fill(i);
  }

  EPHist::EPHist<float> hC(Bins, 0, Bins);
  hC.AddAtomic(hA);
  hC.AddAtomic(hB);

  hA.AddAtomic(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(hA.GetBinContent(i), 2);
    EXPECT_FLOAT_EQ(hB.GetBinContent(i), 1);
    EXPECT_FLOAT_EQ(hC.GetBinContent(i), 2);
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
    h1.FillAtomic(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(FloatRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<float> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, AddAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> hA(Bins, 0, Bins);
  EPHist::EPHist<double> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
    hB.Fill(i);
  }

  EPHist::EPHist<double> hC(Bins, 0, Bins);
  hC.AddAtomic(hA);
  hC.AddAtomic(hB);

  hA.AddAtomic(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(hA.GetBinContent(i), 2);
    EXPECT_FLOAT_EQ(hB.GetBinContent(i), 1);
    EXPECT_FLOAT_EQ(hC.GetBinContent(i), 2);
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
    h1.FillAtomic(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<double> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1.GetBinContent(i), 0.5 + i * 0.1);
  }
}

TEST(DoubleBinWithErrorRegular1D, AddAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> hA(Bins, 0, Bins);
  EPHist::EPHist<EPHist::DoubleBinWithError> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i, EPHist::Weight(0.5 + i * 0.1));
    hB.Fill(i, EPHist::Weight(1.5 + i * 0.2));
  }

  EPHist::EPHist<EPHist::DoubleBinWithError> hC(Bins, 0, Bins);
  hC.AddAtomic(hA);
  hC.AddAtomic(hB);

  hA.AddAtomic(hB);

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

TEST(DoubleBinWithErrorRegular1D, FillAtomic) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
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
    auto &binWithError = h1.GetBinContent(i);
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
    auto &binWithError = h1.GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, FillAtomicTupleWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic(std::make_tuple(i), EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST(DoubleBinWithErrorRegular1D, TemplatedFillAtomicWeight) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<EPHist::DoubleBinWithError> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.FillAtomic<EPHist::RegularAxis>(i, EPHist::Weight(0.5 + i * 0.1));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1.GetBinContent(i);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}
