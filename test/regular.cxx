// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/BinIndex.hxx>
#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>

#include <gtest/gtest.h>

#include <limits>
#include <tuple>
#include <variant>

TEST(RegularAxis, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(axis.GetLow(), 0);
  EXPECT_EQ(axis.GetHigh(), Bins);

  axis = EPHist::RegularAxis(Bins, 0, Bins, /*enableFlowBins=*/false);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetTotalNumBins(), Bins);
}

TEST(RegularAxis, Equality) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axisA(Bins, 0, Bins);
  EPHist::RegularAxis axisANoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);
  EPHist::RegularAxis axisB(Bins, 0, Bins);
  EPHist::RegularAxis axisC(Bins / 2, 0, Bins);
  EPHist::RegularAxis axisD(Bins, 0, Bins / 2);
  EPHist::RegularAxis axisE(Bins, Bins / 2, Bins);

  EXPECT_TRUE(axisA == axisA);
  EXPECT_TRUE(axisA == axisB);
  EXPECT_TRUE(axisB == axisA);

  EXPECT_FALSE(axisA == axisANoFlowBins);

  EXPECT_FALSE(axisA == axisC);
  EXPECT_FALSE(axisA == axisD);
  EXPECT_FALSE(axisA == axisE);

  EXPECT_FALSE(axisC == axisD);
  EXPECT_FALSE(axisC == axisE);

  EXPECT_FALSE(axisD == axisE);
  EXPECT_FALSE(axisE == axisD);
}

TEST(RegularAxis, GetBin) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);

  {
    auto underflow = EPHist::BinIndex::Underflow();
    auto axisBin = axis.GetBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto axisBin = axis.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(i);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Out of bounds
  {
    auto axisBin = axis.GetBin(Bins);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(Bins);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  {
    auto overflow = EPHist::BinIndex::Overflow();
    auto axisBin = axis.GetBin(overflow);
    EXPECT_TRUE(axisBin.second);
    EXPECT_EQ(axisBin.first, Bins + 1);
    axisBin = axisNoFlowBins.GetBin(overflow);
    EXPECT_FALSE(axisBin.second);
  }

  {
    EPHist::BinIndex invalid;
    auto axisBin = axis.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
    axisBin = axisNoFlowBins.GetBin(invalid);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(RegularAxis, ComputeBin) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);

  // Underflow
  static constexpr double NegativeInfinity =
      -std::numeric_limits<double>::infinity();
  static constexpr double UnderflowLarge = -static_cast<double>(Bins);
  static constexpr double UnderflowSmall = -0.1;
  for (double underflow : {NegativeInfinity, UnderflowLarge, UnderflowSmall}) {
    auto axisBin = axis.ComputeBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(underflow);
    EXPECT_EQ(axisBin.first, Bins);
    EXPECT_FALSE(axisBin.second);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto axisBin = axis.ComputeBin(i + 0.5);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(i + 0.5);
    EXPECT_EQ(axisBin.first, i);
    EXPECT_TRUE(axisBin.second);
  }

  // Overflow
  static constexpr double PositiveInfinity =
      std::numeric_limits<double>::infinity();
  static constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
  static constexpr double OverflowLarge = static_cast<double>(Bins * 2);
  static constexpr double OverflowSmall = Bins + 0.1;
  for (double overflow :
       {PositiveInfinity, NaN, OverflowLarge, OverflowSmall}) {
    auto axisBin = axis.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, Bins + 1);
    EXPECT_TRUE(axisBin.second);
    axisBin = axisNoFlowBins.ComputeBin(overflow);
    EXPECT_EQ(axisBin.first, Bins + 1);
    EXPECT_FALSE(axisBin.second);
  }
}

TEST(RegularAxis, Slice) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);

  const auto full = EPHist::BinIndexRange::Full(Bins);
  const auto full0 = EPHist::BinIndexRange::Full(0);
  for (auto &&a : {axis, axisNoFlowBins}) {
    for (auto &&f : {full, full0}) {
      const auto slice = a.Slice(f);
      EXPECT_TRUE(slice.AreFlowBinsEnabled());
      EXPECT_EQ(slice.GetNumBins(), Bins);
      EXPECT_EQ(slice.GetTotalNumBins(), Bins + 2);
      EXPECT_FLOAT_EQ(slice.GetLow(), 0);
      EXPECT_FLOAT_EQ(slice.GetHigh(), Bins);
    }
  }

  const auto bin0 = EPHist::BinIndex(0);
  const auto bin20 = EPHist::BinIndex(Bins);
  const auto inner = EPHist::BinIndexRange(bin0, bin20);
  for (auto &&a : {axis, axisNoFlowBins}) {
    const auto slice = a.Slice(inner);
    EXPECT_TRUE(slice.AreFlowBinsEnabled());
    EXPECT_EQ(slice.GetNumBins(), Bins);
    EXPECT_EQ(slice.GetTotalNumBins(), Bins + 2);
    EXPECT_FLOAT_EQ(slice.GetLow(), 0);
    EXPECT_FLOAT_EQ(slice.GetHigh(), Bins);
  }

  const auto bin5 = EPHist::BinIndex(5);
  const auto bin15 = EPHist::BinIndex(15);
  const auto range = EPHist::BinIndexRange(bin5, bin15);
  for (auto &&a : {axis, axisNoFlowBins}) {
    const auto slice = a.Slice(range);
    EXPECT_TRUE(slice.AreFlowBinsEnabled());
    EXPECT_EQ(slice.GetNumBins(), 10);
    EXPECT_EQ(slice.GetTotalNumBins(), 12);
    EXPECT_FLOAT_EQ(slice.GetLow(), 5);
    EXPECT_FLOAT_EQ(slice.GetHigh(), 15);
  }
}

TEST(IntRegular1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EXPECT_EQ(h1.GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  const auto &axes = h1.GetAxes();
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 0);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::RegularAxis>(axes[0]).GetNumBins(), Bins);

  EPHist::RegularAxis axis(2 * Bins, 0, Bins);
  h1 = EPHist::EPHist<int>(axis);
  EXPECT_EQ(h1.GetTotalNumBins(), 2 * Bins + 2);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 0);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::RegularAxis>(axes[0]).GetNumBins(), 2 * Bins);
}

TEST(IntRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.Fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }
  h1.Fill(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillDiscard) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins,
                           /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }
  h1.Fill(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillOnlyInner) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
    h1NoFlowBins.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1.GetBinContentAt(EPHist::BinIndex(i)), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContentAt(EPHist::BinIndex(i)), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}

TEST(IntRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.Fill(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }
  h1.Fill(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillTupleDiscard) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins,
                           /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(std::make_tuple(-100));
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }
  h1.Fill(std::make_tuple(100));

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillTupleOnlyInner) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
    h1NoFlowBins.Fill(std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}

TEST(IntRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  h1.Fill<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i);
  }
  h1.Fill<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, TemplatedFillDiscard) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins,
                           /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1(axis);

  h1.Fill<EPHist::RegularAxis>(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i);
  }
  h1.Fill<EPHist::RegularAxis>(100);

  for (std::size_t i = 0; i < h1.GetTotalNumBins(); i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, TemplatedFillOnlyInner) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EPHist::RegularAxis axisNoFlowBins(Bins, 0, Bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i);
    h1NoFlowBins.Fill<EPHist::RegularAxis>(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
    EXPECT_EQ(h1NoFlowBins.GetBinContent(i), 1);
  }
  EXPECT_EQ(h1.GetBinContent(Bins), 0);
  EXPECT_EQ(h1.GetBinContent(Bins + 1), 0);
}

TEST(IntRegular2D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h2({axis, axis});
  EXPECT_EQ(h2.GetTotalNumBins(), (Bins + 2) * (Bins + 2));
  EXPECT_EQ(h2.GetNumDimensions(), 2);
  const auto &axes = h2.GetAxes();
  EXPECT_EQ(axes.size(), 2);

  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  h2 = EPHist::EPHist<int>({axis, axisY});
  EXPECT_EQ(h2.GetTotalNumBins(), (Bins + 2) * (BinsY + 2));
  EXPECT_EQ(h2.GetNumDimensions(), 2);
  EXPECT_EQ(axes.size(), 2);
}

TEST(IntRegular2D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill(x, y);
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, FillDiscard) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX,
                            /*enableFlowBins=*/false);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY,
                            /*enableFlowBins=*/false);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill(x, y);
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, FillOnlyInner) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill(x, y);
    }
  }

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + y), 1);
      EXPECT_EQ(h2.GetBinContentAt(x, y), 1);
    }
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY), 0);
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY + 1), 0);
    EXPECT_EQ(h2.GetBinContentAt(x, EPHist::BinIndex::Underflow()), 0);
    EXPECT_EQ(h2.GetBinContentAt(x, EPHist::BinIndex::Overflow()), 0);
  }
  for (std::size_t y = 0; y < BinsY; y++) {
    EXPECT_EQ(h2.GetBinContent(BinsX * (BinsY + 2) + y), 0);
    EXPECT_EQ(h2.GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
    EXPECT_EQ(h2.GetBinContentAt(EPHist::BinIndex::Underflow(), y), 0);
    EXPECT_EQ(h2.GetBinContentAt(EPHist::BinIndex::Overflow(), y), 0);
  }
}

TEST(IntRegular2D, FillTuple) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill(std::make_tuple(x, y));
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, FillTupleDiscard) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX,
                            /*enableFlowBins=*/false);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY,
                            /*enableFlowBins=*/false);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill(std::make_tuple(x, y));
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, FillTupleOnlyInner) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill(std::make_tuple(x, y));
    }
  }

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + y), 1);
    }
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY), 0);
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY + 1), 0);
  }
  for (std::size_t y = 0; y < BinsY; y++) {
    EXPECT_EQ(h2.GetBinContent(BinsX * (BinsY + 2) + y), 0);
    EXPECT_EQ(h2.GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
  }
}

TEST(IntRegular2D, TemplatedFill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(x, y);
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, TemplatedFillDiscard) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX,
                            /*enableFlowBins=*/false);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY,
                            /*enableFlowBins=*/false);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(x, y);
    }
  }

  for (std::size_t i = 0; i < h2.GetTotalNumBins(); i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, TemplatedFillOnlyInner) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(x, y);
    }
  }

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + y), 1);
    }
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY), 0);
    EXPECT_EQ(h2.GetBinContent(x * (BinsY + 2) + BinsY + 1), 0);
  }
  for (std::size_t y = 0; y < BinsY; y++) {
    EXPECT_EQ(h2.GetBinContent(BinsX * (BinsY + 2) + y), 0);
    EXPECT_EQ(h2.GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
  }
}

TEST(IntRegular3D, Constructor) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});
  EXPECT_EQ(h3.GetTotalNumBins(), (BinsX + 2) * (BinsY + 2) * (BinsZ + 2));
  EXPECT_EQ(h3.GetNumDimensions(), 3);
  const auto &axes = h3.GetAxes();
  EXPECT_EQ(axes.size(), 3);
}

TEST(IntRegular3D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      for (int z = -1; z < static_cast<int>(BinsZ) + 1; z++) {
        h3.Fill(x, y, z);
      }
    }
  }

  for (std::size_t i = 0; i < h3.GetTotalNumBins(); i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, FillTuple) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      for (int z = -1; z < static_cast<int>(BinsZ) + 1; z++) {
        h3.Fill(std::make_tuple(x, y, z));
      }
    }
  }

  for (std::size_t i = 0; i < h3.GetTotalNumBins(); i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, TemplatedFill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (int x = -1; x < static_cast<int>(BinsX) + 1; x++) {
    for (int y = -1; y < static_cast<int>(BinsY) + 1; y++) {
      for (int z = -1; z < static_cast<int>(BinsZ) + 1; z++) {
        h3.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis>(
            x, y, z);
      }
    }
  }

  for (std::size_t i = 0; i < h3.GetTotalNumBins(); i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, Constructor) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});
  EXPECT_EQ(h4.GetTotalNumBins(),
            (Bins0 + 2) * (Bins1 + 2) * (Bins2 + 2) * (Bins3 + 2));
  EXPECT_EQ(h4.GetNumDimensions(), 4);
  const auto &axes = h4.GetAxes();
  EXPECT_EQ(axes.size(), 4);
}

TEST(IntRegular4D, Fill) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (int v0 = -1; v0 < static_cast<int>(Bins0) + 1; v0++) {
    for (int v1 = -1; v1 < static_cast<int>(Bins1) + 1; v1++) {
      for (int v2 = -1; v2 < static_cast<int>(Bins2) + 1; v2++) {
        for (int v3 = -1; v3 < static_cast<int>(Bins3) + 1; v3++) {
          h4.Fill(v0, v1, v2, v3);
        }
      }
    }
  }

  for (std::size_t i = 0; i < h4.GetTotalNumBins(); i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, FillTuple) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (int v0 = -1; v0 < static_cast<int>(Bins0) + 1; v0++) {
    for (int v1 = -1; v1 < static_cast<int>(Bins1) + 1; v1++) {
      for (int v2 = -1; v2 < static_cast<int>(Bins2) + 1; v2++) {
        for (int v3 = -1; v3 < static_cast<int>(Bins3) + 1; v3++) {
          h4.Fill(std::make_tuple(v0, v1, v2, v3));
        }
      }
    }
  }

  for (std::size_t i = 0; i < h4.GetTotalNumBins(); i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, TemplatedFill) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (int v0 = -1; v0 < static_cast<int>(Bins0) + 1; v0++) {
    for (int v1 = -1; v1 < static_cast<int>(Bins1) + 1; v1++) {
      for (int v2 = -1; v2 < static_cast<int>(Bins2) + 1; v2++) {
        for (int v3 = -1; v3 < static_cast<int>(Bins3) + 1; v3++) {
          h4.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis,
                  EPHist::RegularAxis>(v0, v1, v2, v3);
        }
      }
    }
  }

  for (std::size_t i = 0; i < h4.GetTotalNumBins(); i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}
