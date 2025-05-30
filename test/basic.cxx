// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

#include <stdexcept>
#include <tuple>

TEST(Basic, MixedAxisTypes) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);

  EPHist::EPHist<int> h({regularAxis, variableBinAxis, regularAxis});
  EXPECT_EQ(h.GetNumDimensions(), 3);

  std::vector<EPHist::AxisVariant> newAxes{variableBinAxis, regularAxis};
  h = EPHist::EPHist<int>(newAxes);
  ASSERT_EQ(h.GetNumDimensions(), 2);

  h.Fill(1, 2);
  h.Fill(std::make_tuple(1, 2));
  h.Fill<EPHist::VariableBinAxis, EPHist::RegularAxis>(1, 2);
}

TEST(Basic, Add) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> hA(Bins, 0, Bins);
  EPHist::EPHist<int> hB(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
    hB.Fill(i);
  }

  EPHist::EPHist<int> hC(Bins, 0, Bins);
  hC.Add(hA);
  hC.Add(hB);

  hA.Add(hB);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(hA.GetBinContent(i), 2);
    EXPECT_EQ(hB.GetBinContent(i), 1);
    EXPECT_EQ(hC.GetBinContent(i), 2);
  }
}

TEST(Basic, AddDifferentDimensions) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_THROW(h1.Add(h2), std::invalid_argument);
}

TEST(Basic, AddDifferentAxisTypes) {
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

  EXPECT_THROW(hA.Add(hB), std::invalid_argument);
}

TEST(Basic, AddUnequalRegularAxis) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> hA(Bins, 0, Bins);
  EPHist::EPHist<int> hB(Bins / 2, 0, Bins);

  EXPECT_THROW(hA.Add(hB), std::invalid_argument);
}

TEST(Basic, AddFlowBins) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> hA(axis);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> hB(axisNoFlowBins);

  EXPECT_THROW(hA.Add(hB), std::invalid_argument);
}

TEST(Basic, Clear) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h.Fill(i);
  }

  h.Clear();

  for (std::size_t i = 0; i < h.GetTotalNumBins(); i++) {
    EXPECT_EQ(h.GetBinContent(i), 0);
  }
}

TEST(Basic, Clone) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> hA(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    hA.Fill(i);
  }

  EPHist::EPHist<int> hB = hA.Clone();
  ASSERT_EQ(hB.GetTotalNumBins(), Bins + 2);
  ASSERT_EQ(hB.GetNumDimensions(), 1);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(hB.GetBinContent(i), 1);
  }

  // Check that we can continue filling the clone.
  for (std::size_t i = 0; i < Bins; i++) {
    hB.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(hA.GetBinContent(i), 1);
    EXPECT_EQ(hB.GetBinContent(i), 2);
  }
}

TEST(Basic, GetBinContentAtNotFound) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  EXPECT_THROW(h1.GetBinContentAt(Bins), std::invalid_argument);
}

TEST(Basic, GetBinContentAtInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.GetBinContentAt(1));
  EXPECT_THROW(h1.GetBinContentAt(1, 2), std::invalid_argument);

  EXPECT_THROW(h2.GetBinContentAt(1), std::invalid_argument);
  EXPECT_NO_THROW(h2.GetBinContentAt(1, 2));
  EXPECT_THROW(h2.GetBinContentAt(1, 2, 3), std::invalid_argument);
}

TEST(Basic, GetBinContentAtArrayInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  std::array<EPHist::BinIndex, 1> a1 = {1};
  std::array<EPHist::BinIndex, 2> a2 = {1, 2};
  std::array<EPHist::BinIndex, 3> a3 = {1, 2, 3};
  EXPECT_NO_THROW(h1.GetBinContentAt(a1));
  EXPECT_THROW(h1.GetBinContentAt(a2), std::invalid_argument);

  EXPECT_THROW(h2.GetBinContentAt(a1), std::invalid_argument);
  EXPECT_NO_THROW(h2.GetBinContentAt(a2));
  EXPECT_THROW(h2.GetBinContentAt(a3), std::invalid_argument);
}

TEST(Basic, FillInvalidArgumentType) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  EXPECT_THROW(h1.Fill("test"), std::invalid_argument);

  // An implicitly convertible type works...
  struct ConvertibleToDouble {
    operator double() const { return 1.0; }
  };
  EXPECT_NO_THROW(h1.Fill(ConvertibleToDouble{}));

  // ... but an explicit conversion operator is not called!
  struct ExplicitlyConvertibleToDouble {
    explicit operator double() const { return 1.0; }
  };
  EXPECT_THROW(h1.Fill(ExplicitlyConvertibleToDouble{}), std::invalid_argument);
}

TEST(Basic, FillInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.Fill(1));
  EXPECT_THROW(h1.Fill(1, 2), std::invalid_argument);

  EXPECT_THROW(h2.Fill(1), std::invalid_argument);
  EXPECT_NO_THROW(h2.Fill(1, 2));
  EXPECT_THROW(h2.Fill(1, 2, 3), std::invalid_argument);
}

TEST(Basic, FillTupleInvalidArgumentType) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  EXPECT_THROW(h1.Fill(std::make_tuple("test")), std::invalid_argument);

  // An implicitly convertible type works...
  struct ConvertibleToDouble {
    operator double() const { return 1.0; }
  };
  EXPECT_NO_THROW(h1.Fill(std::make_tuple(ConvertibleToDouble{})));

  // ... but an explicit conversion operator is not called!
  struct ExplicitlyConvertibleToDouble {
    explicit operator double() const { return 1.0; }
  };
  EXPECT_THROW(h1.Fill(std::make_tuple(ExplicitlyConvertibleToDouble{})),
               std::invalid_argument);
}

TEST(Basic, FillTupleInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.Fill(std::make_tuple(1)));
  EXPECT_THROW(h1.Fill(std::make_tuple(1, 2)), std::invalid_argument);

  EXPECT_THROW(h2.Fill(std::make_tuple(1)), std::invalid_argument);
  EXPECT_NO_THROW(h2.Fill(std::make_tuple(1, 2)));
  EXPECT_THROW(h2.Fill(std::make_tuple(1, 2, 3)), std::invalid_argument);
}

TEST(Basic, TemplatedFillInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  EXPECT_NO_THROW(h1.Fill<EPHist::RegularAxis>(1));
  EXPECT_THROW((h1.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)),
               std::invalid_argument);

  EXPECT_THROW(h2.Fill<EPHist::RegularAxis>(1), std::invalid_argument);
  EXPECT_NO_THROW((h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)));
  EXPECT_THROW(
      (h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis>(
          1, 2, 3)),
      std::invalid_argument);
}

TEST(Basic, TemplatedFillInvalidAxis) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);
  EPHist::EPHist<int> h1(variableBinAxis);
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  EPHist::EPHist<int> h2({regularAxis, regularAxis});

  EXPECT_THROW((h1.Fill<EPHist::RegularAxis>(1)), std::invalid_argument);
  EXPECT_NO_THROW(h1.Fill<EPHist::VariableBinAxis>(1));

  EXPECT_NO_THROW((h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(1, 2)));
  EXPECT_THROW((h2.Fill<EPHist::RegularAxis, EPHist::VariableBinAxis>(1, 2)),
               std::invalid_argument);
  EXPECT_THROW((h2.Fill<EPHist::VariableBinAxis, EPHist::RegularAxis>(1, 2)),
               std::invalid_argument);
  EXPECT_THROW(
      (h2.Fill<EPHist::VariableBinAxis, EPHist::VariableBinAxis>(1, 2)),
      std::invalid_argument);
}

TEST(Basic, FillWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.Fill(1, EPHist::Weight(1)));
  EXPECT_THROW(h1.Fill(1, 2, EPHist::Weight(1)), std::invalid_argument);

  EXPECT_THROW(h2.Fill(1, EPHist::Weight(1)), std::invalid_argument);
  EXPECT_NO_THROW(h2.Fill(1, 2, EPHist::Weight(1)));
  EXPECT_THROW(h2.Fill(1, 2, 3, EPHist::Weight(1)), std::invalid_argument);
}

TEST(Basic, FillTupleWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.Fill(std::make_tuple(1), EPHist::Weight(1)));
  EXPECT_THROW(h1.Fill(std::make_tuple(1, 2), EPHist::Weight(1)),
               std::invalid_argument);

  EXPECT_THROW(h2.Fill(std::make_tuple(1), EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW(h2.Fill(std::make_tuple(1, 2), EPHist::Weight(1)));
  EXPECT_THROW(h2.Fill(std::make_tuple(1, 2, 3), EPHist::Weight(1)),
               std::invalid_argument);
}

TEST(Basic, TemplatedFillWeightInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<double> h1(axis);
  EPHist::EPHist<double> h2({axis, axis});

  EXPECT_NO_THROW(h1.Fill<EPHist::RegularAxis>(1, EPHist::Weight(1)));
  EXPECT_THROW((h1.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(
                   1, 2, EPHist::Weight(1))),
               std::invalid_argument);

  EXPECT_THROW(h2.Fill<EPHist::RegularAxis>(1, EPHist::Weight(1)),
               std::invalid_argument);
  EXPECT_NO_THROW((h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(
      1, 2, EPHist::Weight(1))));
  EXPECT_THROW(
      (h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis>(
          1, 2, 3, EPHist::Weight(1))),
      std::invalid_argument);
}

TEST(Basic, TemplatedFillWeightInvalidAxis) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);
  EPHist::VariableBinAxis variableBinAxis(bins);
  EPHist::EPHist<double> h1(variableBinAxis);
  EPHist::RegularAxis regularAxis(Bins, 0, Bins);
  EPHist::EPHist<double> h2({regularAxis, regularAxis});

  EXPECT_THROW((h1.Fill<EPHist::RegularAxis>(1, EPHist::Weight(1))),
               std::invalid_argument);
  EXPECT_NO_THROW(h1.Fill<EPHist::VariableBinAxis>(1, EPHist::Weight(1)));

  EXPECT_NO_THROW((h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(
      1, 2, EPHist::Weight(1))));
  EXPECT_THROW((h2.Fill<EPHist::RegularAxis, EPHist::VariableBinAxis>(
                   1, 2, EPHist::Weight(1))),
               std::invalid_argument);
  EXPECT_THROW((h2.Fill<EPHist::VariableBinAxis, EPHist::RegularAxis>(
                   1, 2, EPHist::Weight(1))),
               std::invalid_argument);
  EXPECT_THROW((h2.Fill<EPHist::VariableBinAxis, EPHist::VariableBinAxis>(
                   1, 2, EPHist::Weight(1))),
               std::invalid_argument);
}
