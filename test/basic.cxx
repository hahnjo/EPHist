// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>

#include <gtest/gtest.h>

#include <stdexcept>
#include <tuple>

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
