// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/BinIndexRange.hxx>

#include <gtest/gtest.h>

#include <iterator>
#include <stdexcept>
#include <vector>

TEST(BinIndex, Constructor) {
  EPHist::BinIndex invalid;
  EXPECT_TRUE(invalid.IsInvalid());

  auto index = EPHist::BinIndex(0);
  EXPECT_TRUE(index.IsNormal());
  EXPECT_EQ(index.GetIndex(), 0);

  auto underflow = EPHist::BinIndex::Underflow();
  EXPECT_FALSE(underflow.IsNormal());
  EXPECT_TRUE(underflow.IsUnderflow());

  auto overflow = EPHist::BinIndex::Overflow();
  EXPECT_FALSE(overflow.IsNormal());
  EXPECT_TRUE(overflow.IsOverflow());
}

TEST(BinIndex, Plus) {
  auto index1 = EPHist::BinIndex(1);
  ASSERT_EQ(index1.GetIndex(), 1);

  {
    auto index2 = index1;
    EXPECT_EQ((++index2).GetIndex(), 2);
    EXPECT_EQ(index2.GetIndex(), 2);
  }

  {
    auto index2 = index1;
    EXPECT_EQ((index2++).GetIndex(), 1);
    EXPECT_EQ(index2.GetIndex(), 2);
  }

  {
    auto index3 = index1;
    EXPECT_EQ((index3 += 2).GetIndex(), 3);
    EXPECT_EQ(index3.GetIndex(), 3);
  }

  {
    auto index3 = index1 + 2;
    EXPECT_EQ(index3.GetIndex(), 3);
  }
}

TEST(BinIndex, Minus) {
  auto index3 = EPHist::BinIndex(3);
  ASSERT_EQ(index3.GetIndex(), 3);

  {
    auto index2 = index3;
    EXPECT_EQ((--index2).GetIndex(), 2);
    EXPECT_EQ(index2.GetIndex(), 2);
  }

  {
    auto index2 = index3;
    EXPECT_EQ((index2--).GetIndex(), 3);
    EXPECT_EQ(index2.GetIndex(), 2);
  }

  {
    auto index1 = index3;
    EXPECT_EQ((index1 -= 2).GetIndex(), 1);
    EXPECT_EQ(index1.GetIndex(), 1);
  }

  {
    auto index1 = index3 - 2;
    EXPECT_EQ(index1.GetIndex(), 1);
  }
}

TEST(BinIndex, Equality) {
  auto index = EPHist::BinIndex(1);
  EXPECT_EQ(index, EPHist::BinIndex(1));
  index++;
  EXPECT_EQ(index, EPHist::BinIndex(2));
  EXPECT_NE(index, EPHist::BinIndex(3));

  auto underflow = EPHist::BinIndex::Underflow();
  EXPECT_EQ(underflow, EPHist::BinIndex::Underflow());
  EXPECT_NE(index, underflow);

  auto overflow = EPHist::BinIndex::Overflow();
  EXPECT_EQ(overflow, EPHist::BinIndex::Overflow());
  EXPECT_NE(index, overflow);
  EXPECT_NE(underflow, overflow);
}

TEST(BinIndex, Relation) {
  auto index1 = EPHist::BinIndex(1);
  auto index2 = EPHist::BinIndex(2);
  EXPECT_LT(index1, index2);
  EXPECT_LE(index1, index2);
  EXPECT_GT(index2, index1);
  EXPECT_GE(index2, index1);

  auto underflow = EPHist::BinIndex::Underflow();
  EXPECT_LE(underflow, EPHist::BinIndex::Underflow());
  EXPECT_GE(underflow, EPHist::BinIndex::Underflow());
  EXPECT_FALSE(index1 < underflow);
  EXPECT_FALSE(index1 <= underflow);
  EXPECT_FALSE(index1 > underflow);
  EXPECT_FALSE(index1 >= underflow);

  auto overflow = EPHist::BinIndex::Overflow();
  EXPECT_LE(overflow, EPHist::BinIndex::Overflow());
  EXPECT_GE(overflow, EPHist::BinIndex::Overflow());
  EXPECT_FALSE(index1 < overflow);
  EXPECT_FALSE(index1 <= overflow);
  EXPECT_FALSE(index1 > overflow);
  EXPECT_FALSE(index1 >= overflow);

  EXPECT_FALSE(underflow < overflow);
  EXPECT_FALSE(underflow <= overflow);
  EXPECT_FALSE(underflow > overflow);
  EXPECT_FALSE(underflow >= overflow);
}

TEST(BinIndexRange, Constructor) {
  const EPHist::BinIndexRange invalid;
  EXPECT_TRUE(invalid.GetBegin().IsInvalid());
  EXPECT_TRUE(invalid.GetEnd().IsInvalid());

  const auto index0 = EPHist::BinIndex(0);
  const auto range0 = EPHist::BinIndexRange(index0, index0);
  EXPECT_EQ(range0.GetBegin(), index0);
  EXPECT_EQ(range0.GetEnd(), index0);

  const auto range01 = EPHist::BinIndexRange(index0, EPHist::BinIndex(1));
  EXPECT_EQ(range01.GetBegin(), index0);
  EXPECT_EQ(range01.GetEnd(), EPHist::BinIndex(1));

  // Swapped begin and end
  EXPECT_THROW(EPHist::BinIndexRange(EPHist::BinIndex(1), index0),
               std::invalid_argument);

  const auto underflow = EPHist::BinIndex::Underflow();
  const auto overflow = EPHist::BinIndex::Overflow();
  EXPECT_THROW(EPHist::BinIndexRange(underflow, index0), std::invalid_argument);
  EXPECT_THROW(EPHist::BinIndexRange(overflow, index0), std::invalid_argument);
  EXPECT_THROW(EPHist::BinIndexRange(index0, underflow), std::invalid_argument);
  EXPECT_THROW(EPHist::BinIndexRange(index0, overflow), std::invalid_argument);
}

TEST(BinIndexRange, Empty) {
  const auto index0 = EPHist::BinIndex(0);
  const auto empty = EPHist::BinIndexRange(index0, index0);
  EXPECT_EQ(empty.begin(), empty.end());
  EXPECT_EQ(std::distance(empty.begin(), empty.end()), 0);

  EXPECT_FALSE(empty.IsFull());
  EXPECT_EQ(empty.GetNormalRange(), empty);
}

TEST(BinIndexRange, Normal) {
  const auto index0 = EPHist::BinIndex(0);
  const auto range01 = EPHist::BinIndexRange(index0, EPHist::BinIndex(1));
  EXPECT_EQ(std::distance(range01.begin(), range01.end()), 1);
  auto range01It = range01.begin();
  EXPECT_EQ(*range01It, index0);
  range01It++;
  EXPECT_EQ(range01It, range01.end());

  EXPECT_FALSE(range01.IsFull());
  EXPECT_EQ(range01.GetNormalRange(), range01);
}

TEST(BinIndexRange, Full) {
  const auto full = EPHist::BinIndexRange::Full(/*numBins*/ 10);
  EXPECT_EQ(full.GetBegin(), EPHist::BinIndex::Underflow());
  EXPECT_EQ(full.GetEnd(), EPHist::BinIndex());
  EXPECT_EQ(std::distance(full.begin(), full.end()), 12);

  const std::vector binIndexes(full.begin(), full.end());
  ASSERT_EQ(binIndexes.size(), 12);
  EXPECT_TRUE(binIndexes.front().IsUnderflow());
  EXPECT_TRUE(binIndexes.back().IsOverflow());

  EXPECT_TRUE(full.IsFull());
  const auto normal = full.GetNormalRange();
  EXPECT_EQ(normal.GetBegin(), EPHist::BinIndex(0));
  EXPECT_EQ(normal.GetEnd(), EPHist::BinIndex(10));

  const auto normal20 = full.GetNormalRange(20);
  EXPECT_EQ(normal20.GetBegin(), EPHist::BinIndex(0));
  EXPECT_EQ(normal20.GetEnd(), EPHist::BinIndex(20));
}
