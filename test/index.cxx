// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>

#include <gtest/gtest.h>

TEST(BinIndex, Constructor) {
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
