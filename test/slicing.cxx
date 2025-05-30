// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/BinIndexRange.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>

#include <gtest/gtest.h>

TEST(Slicing, SliceInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  const auto full = EPHist::BinIndexRange::Full(Bins);

  EXPECT_NO_THROW(h1.Slice(full));
  EXPECT_THROW(h1.Slice(full, full), std::invalid_argument);

  EXPECT_THROW(h2.Slice(full), std::invalid_argument);
  EXPECT_NO_THROW(h2.Slice(full, full));
  EXPECT_THROW(h2.Slice(full, full, full), std::invalid_argument);
}

TEST(Slicing, SliceArrayInvalidNumberOfArguments) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h1(axis);
  ASSERT_EQ(h1.GetNumDimensions(), 1);
  EPHist::EPHist<int> h2({axis, axis});
  ASSERT_EQ(h2.GetNumDimensions(), 2);

  const auto full = EPHist::BinIndexRange::Full(Bins);
  std::array<EPHist::BinIndexRange, 1> full1 = {full};
  std::array<EPHist::BinIndexRange, 2> full2 = {full, full};
  std::array<EPHist::BinIndexRange, 3> full3 = {full, full, full};

  EXPECT_NO_THROW(h1.Slice(full1));
  EXPECT_THROW(h1.Slice(full2), std::invalid_argument);

  EXPECT_THROW(h2.Slice(full1), std::invalid_argument);
  EXPECT_NO_THROW(h2.Slice(full2));
  EXPECT_THROW(h2.Slice(full3), std::invalid_argument);
}

TEST(Slicing, Slice1D) {
  // https://github.com/scikit-hep/uhi/blob/bd2de58dde1de75881a71b7445d538c27e1d0307/src/uhi/testing/indexing.py#L31-L32
  static constexpr std::size_t Bins = 10;
  EPHist::EPHist<int> h(Bins, 0, 1);
  // The underflow bin has 3.
  h.Fill(-1);
  h.Fill(-1);
  h.Fill(-1);
  // Each bin has 2 more than the one before, starting with 0.
  for (std::size_t i = 0; i < Bins; i++) {
    const double d = (0.5 + i) / Bins;
    for (std::size_t j = 0; j < 2 * i; j++) {
      h.Fill(d);
    }
  }
  // The overflow bin has 1.
  h.Fill(2);

  // Check the original histogram.
  const auto underflow = EPHist::BinIndex::Underflow();
  const auto overflow = EPHist::BinIndex::Overflow();
  ASSERT_EQ(h.GetBinContentAt(underflow), 3);
  for (std::size_t i = 0; i < Bins; i++) {
    ASSERT_EQ(h.GetBinContentAt(i), 2 * i);
  }
  ASSERT_EQ(h.GetBinContentAt(overflow), 1);

  {
    // test_slicing_all
    auto full = EPHist::BinIndexRange::Full(Bins);
    auto normal = full.GetNormalRange();
    for (auto &&r : {full, normal}) {
      auto sliced = h.Slice(r);
      ASSERT_EQ(sliced.GetNumDimensions(), 1);
      ASSERT_EQ(sliced.GetTotalNumBins(), Bins + 2);
      EXPECT_EQ(h.GetBinContentAt(underflow), 3);
      for (std::size_t i = 0; i < Bins; i++) {
        EXPECT_EQ(h.GetBinContentAt(i), 2 * i);
      }
      EXPECT_EQ(h.GetBinContentAt(overflow), 1);
    }
  }

  {
    // test_slicing_closed
    EPHist::BinIndexRange closedRange(2, 4);
    auto slicingClosed = h.Slice(closedRange);
    ASSERT_EQ(slicingClosed.GetNumDimensions(), 1);
    ASSERT_EQ(slicingClosed.GetTotalNumBins(), 4);
    EXPECT_EQ(slicingClosed.GetBinContentAt(underflow), 5);
    EXPECT_EQ(slicingClosed.GetBinContentAt(0), 4);
    EXPECT_EQ(slicingClosed.GetBinContentAt(1), 6);
    EXPECT_EQ(slicingClosed.GetBinContentAt(overflow), 79);
  }

  {
    // test_slicing_open_upper
    EPHist::BinIndexRange openUpperRange(5, Bins);
    auto slicingOpenUpper = h.Slice(openUpperRange);
    ASSERT_EQ(slicingOpenUpper.GetNumDimensions(), 1);
    ASSERT_EQ(slicingOpenUpper.GetTotalNumBins(), 7);
    EXPECT_EQ(slicingOpenUpper.GetBinContentAt(underflow), 23);
    EXPECT_EQ(slicingOpenUpper.GetBinContentAt(0), 10);
    EXPECT_EQ(slicingOpenUpper.GetBinContentAt(4), 18);
    EXPECT_EQ(slicingOpenUpper.GetBinContentAt(overflow), 1);
  }

  {
    // test_slicing_open_lower
    EPHist::BinIndexRange openLowerRange(0, 5);
    auto slicingOpenLower = h.Slice(openLowerRange);
    ASSERT_EQ(slicingOpenLower.GetNumDimensions(), 1);
    ASSERT_EQ(slicingOpenLower.GetTotalNumBins(), 7);
    EXPECT_EQ(slicingOpenLower.GetBinContentAt(underflow), 3);
    EXPECT_EQ(slicingOpenLower.GetBinContentAt(0), 0);
    EXPECT_EQ(slicingOpenLower.GetBinContentAt(4), 8);
    EXPECT_EQ(slicingOpenLower.GetBinContentAt(overflow), 71);
  }
}

TEST(Slicing, Slice2D) {
  static constexpr std::size_t BinsX = 3;
  EPHist::RegularAxis axisX(BinsX, 0, 1);
  static constexpr std::size_t BinsY = 5;
  EPHist::RegularAxis axisY(BinsY, 0, 1);
  EPHist::EPHist<int> h({axisX, axisY});
  // The contents are x + 2y + 1, where x and y are the bin indices.
  for (std::size_t x = 0; x < BinsX; x++) {
    const double dX = (0.5 + x) / BinsX;
    for (std::size_t y = 0; y < BinsY; y++) {
      const double dY = (0.5 + y) / BinsY;
      for (std::size_t j = 0; j < x + 2 * y + 1; j++) {
        h.Fill(dX, dY);
      }
    }
  }

  // Check the original histogram.
  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      ASSERT_EQ(h.GetBinContentAt(x, y), x + 2 * y + 1);
    }
  }

  {
    // test_slicing_all
    auto fullX = EPHist::BinIndexRange::Full(BinsX);
    auto fullY = EPHist::BinIndexRange::Full(BinsY);
    for (auto &&rX : {fullX, fullX.GetNormalRange()}) {
      for (auto &&rY : {fullY, fullY.GetNormalRange()}) {
        auto sliced = h.Slice(rX, rY);
        ASSERT_EQ(sliced.GetNumDimensions(), 2);
        ASSERT_EQ(sliced.GetTotalNumBins(), (BinsX + 2) * (BinsY + 2));
        for (std::size_t x = 0; x < BinsX; x++) {
          for (std::size_t y = 0; y < BinsY; y++) {
            EXPECT_EQ(h.GetBinContentAt(x, y), x + 2 * y + 1);
          }
        }
      }
    }
  }

  {
    auto rangeX = EPHist::BinIndexRange(1, 2);
    auto rangeY = EPHist::BinIndexRange(2, 4);
    auto sliced = h.Slice(rangeX, rangeY);
    ASSERT_EQ(sliced.GetNumDimensions(), 2);
    ASSERT_EQ(sliced.GetTotalNumBins(), 12);
    const auto underflow = EPHist::BinIndex::Underflow();
    const auto overflow = EPHist::BinIndex::Overflow();
    // [0, 0], [0, 1]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, underflow), 4);
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 0), 5);
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 1), 7);
    // [0, 4]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, overflow), 9);
    // [1, 0], [1, 1]
    EXPECT_EQ(sliced.GetBinContentAt(0, underflow), 6);
    EXPECT_EQ(sliced.GetBinContentAt(0, 0), 6);
    EXPECT_EQ(sliced.GetBinContentAt(0, 1), 8);
    // [1, 4]
    EXPECT_EQ(sliced.GetBinContentAt(0, overflow), 10);
    // [2, 0], [2, 1]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, underflow), 8);
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 0), 7);
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 1), 9);
    // [2, 4]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, overflow), 11);
  }
}

TEST(Slicing, Slice3D) {
  static constexpr std::size_t BinsX = 3;
  EPHist::RegularAxis axisX(BinsX, 0, 1);
  static constexpr std::size_t BinsY = 5;
  EPHist::RegularAxis axisY(BinsY, 0, 1);
  static constexpr std::size_t BinsZ = 8;
  EPHist::RegularAxis axisZ(BinsZ, 0, 1);
  EPHist::EPHist<int> h({axisX, axisY, axisZ});
  // The contents are x + 2y + 1, where x and y are the bin indices.
  for (std::size_t x = 0; x < BinsX; x++) {
    const double dX = (0.5 + x) / BinsX;
    for (std::size_t y = 0; y < BinsY; y++) {
      const double dY = (0.5 + y) / BinsY;
      for (std::size_t z = 0; z < BinsZ; z++) {
        const double dZ = (0.5 + z) / BinsZ;
        for (std::size_t j = 0; j < x + 2 * y + 3 * z + 1; j++) {
          h.Fill(dX, dY, dZ);
        }
      }
    }
  }

  // Check the original histogram.
  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      for (std::size_t z = 0; z < BinsZ; z++) {
        ASSERT_EQ(h.GetBinContentAt(x, y, z), x + 2 * y + 3 * z + 1);
      }
    }
  }

  {
    // test_slicing_all
    auto fullX = EPHist::BinIndexRange::Full(BinsX);
    auto fullY = EPHist::BinIndexRange::Full(BinsY);
    auto fullZ = EPHist::BinIndexRange::Full(BinsZ);
    for (auto &&rX : {fullX, fullX.GetNormalRange()}) {
      for (auto &&rY : {fullY, fullY.GetNormalRange()}) {
        for (auto &&rZ : {fullZ, fullZ.GetNormalRange()}) {
          auto sliced = h.Slice(rX, rY, rZ);
          ASSERT_EQ(sliced.GetNumDimensions(), 3);
          ASSERT_EQ(sliced.GetTotalNumBins(),
                    (BinsX + 2) * (BinsY + 2) * (BinsZ + 2));
          for (std::size_t x = 0; x < BinsX; x++) {
            for (std::size_t y = 0; y < BinsY; y++) {
              for (std::size_t z = 0; z < BinsZ; z++) {
                EXPECT_EQ(h.GetBinContentAt(x, y, z), x + 2 * y + 3 * z + 1);
              }
            }
          }
        }
      }
    }
  }

  {
    auto rangeX = EPHist::BinIndexRange(1, 2);
    auto rangeY = EPHist::BinIndexRange(2, 4);
    auto rangeZ = EPHist::BinIndexRange(5, 6);
    auto sliced = h.Slice(rangeX, rangeY, rangeZ);
    ASSERT_EQ(sliced.GetNumDimensions(), 3);
    ASSERT_EQ(sliced.GetTotalNumBins(), 36);
    const auto underflow = EPHist::BinIndex::Underflow();
    const auto overflow = EPHist::BinIndex::Overflow();
    // [0, 0:2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, underflow, underflow), 80);
    // [0, 0:2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, underflow, 0), 34);
    // [0, 0:2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, underflow, overflow), 86);
    // [0, 2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 0, underflow), 55);
    // [0, 2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 0, 0), 20);
    // [0, 2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 0, overflow), 49);
    // [0, 3, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 1, underflow), 65);
    // [0, 3, 5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 1, 0), 22);
    // [0, 3, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, 1, overflow), 53);
    // [0, 4, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, overflow, underflow), 75);
    // [0, 4, 5]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, overflow, 0), 24);
    // [0, 4, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(underflow, overflow, overflow), 57);

    // [1, 0:2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(0, underflow, underflow), 90);
    // [1, 0:2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(0, underflow, 0), 36);
    // [1, 0:2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(0, underflow, overflow), 90);
    // [1, 2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(0, 0, underflow), 60);
    // [1, 2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(0, 0, 0), 21);
    // [1, 2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(0, 0, overflow), 51);
    // [1, 3, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(0, 1, underflow), 70);
    // [1, 3, 5]
    EXPECT_EQ(sliced.GetBinContentAt(0, 1, 0), 23);
    // [1, 3, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(0, 1, overflow), 55);
    // [1, 4, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(0, overflow, underflow), 80);
    // [1, 4, 5]
    EXPECT_EQ(sliced.GetBinContentAt(0, overflow, 0), 25);
    // [1, 4, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(0, overflow, overflow), 59);

    // [2, 0:2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, underflow, underflow), 100);
    // [2, 0:2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, underflow, 0), 38);
    // [2, 0:2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, underflow, overflow), 94);
    // [2, 2, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 0, underflow), 65);
    // [2, 2, 5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 0, 0), 22);
    // [2, 2, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 0, overflow), 53);
    // [2, 3, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 1, underflow), 75);
    // [2, 3, 5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 1, 0), 24);
    // [2, 3, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, 1, overflow), 57);
    // [2, 4, 0:5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, overflow, underflow), 85);
    // [2, 4, 5]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, overflow, 0), 26);
    // [2, 4, 6:8]
    EXPECT_EQ(sliced.GetBinContentAt(overflow, overflow, overflow), 61);
  }
}
