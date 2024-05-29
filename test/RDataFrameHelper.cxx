// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/Util/RDataFrameHelper.hxx>

#include <ROOT/RDataFrame.hxx>

#include <gtest/gtest.h>

TEST(EPHistFillHelper, IntRegular1D) {
  // The same histograms as IntRegular1D.FillOnlyInner in regular.cxx
  static constexpr std::size_t Bins = 20;
  ROOT::RDataFrame df(Bins);

  EPHist::Util::EPHistFillHelper<int> helper(df.GetNSlots(), Bins, 0, Bins);
  auto h1 = df.Book<ULong64_t>(std::move(helper), {"rdfentry_"});

  EPHist::RegularAxis axisNoUnderflowOverflow(
      Bins, 0, Bins, /*enableUnderflowOverflowBins=*/false);
  EPHist::Util::EPHistFillAddHelper<int> helperNoUnderflowOverflow(
      df.GetNSlots(), axisNoUnderflowOverflow);
  auto h1NoUnderflowOverflow =
      df.Book<ULong64_t>(std::move(helperNoUnderflowOverflow), {"rdfentry_"});

  ASSERT_EQ(h1->GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(h1->GetNumDimensions(), 1);
  ASSERT_EQ(h1NoUnderflowOverflow->GetTotalNumBins(), Bins);
  EXPECT_EQ(h1NoUnderflowOverflow->GetNumDimensions(), 1);

  EXPECT_EQ(h1->GetBinContent(0), 0);
  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1->GetBinContent(i + 1), 1);
    EXPECT_EQ(h1NoUnderflowOverflow->GetBinContent(i), 1);
  }
  EXPECT_EQ(h1->GetBinContent(Bins + 1), 0);
}

TEST(EPHistFillHelper, IntRegular2D) {
  // The same histogram as IntRegular2D.FillOnlyInner in regular.cxx
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);

  auto df = ROOT::RDataFrame(BinsX * BinsY)
                .Define("x", [&](ULong64_t entry) { return entry / BinsY; },
                        {"rdfentry_"})
                .Define("y", [&](ULong64_t entry) { return entry % BinsY; },
                        {"rdfentry_"});

  // TODO: Ideally accept a brace-enclosed initializer list
  std::vector<EPHist::AxisVariant> axes{axisX, axisY};
  EPHist::Util::EPHistFillHelper<int> helper(df.GetNSlots(), axes);
  auto h2 = df.Book<ULong64_t, ULong64_t>(std::move(helper), {"x", "y"});

  EXPECT_EQ(h2->GetTotalNumBins(), (BinsX + 2) * (BinsY + 2));
  EXPECT_EQ(h2->GetNumDimensions(), 2);

  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent(y), 0);
  }
  for (std::size_t x = 0; x < BinsX; x++) {
    EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2)), 0);
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2) + y + 1), 1);
    }
    EXPECT_EQ(h2->GetBinContent((x + 2) * (BinsY + 2) - 1), 0);
  }
  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
  }
}

TEST(EPHistFillAddHelper, IntRegular1D) {
  // The same histograms as IntRegular1D.FillOnlyInner in regular.cxx
  static constexpr std::size_t Bins = 20;
  ROOT::RDataFrame df(Bins);

  EPHist::Util::EPHistFillAddHelper<int> helper(df.GetNSlots(), Bins, 0, Bins);
  auto h1 = df.Book<ULong64_t>(std::move(helper), {"rdfentry_"});

  EPHist::RegularAxis axisNoUnderflowOverflow(
      Bins, 0, Bins, /*enableUnderflowOverflowBins=*/false);
  EPHist::Util::EPHistFillAddHelper<int> helperNoUnderflowOverflow(
      df.GetNSlots(), axisNoUnderflowOverflow);
  auto h1NoUnderflowOverflow =
      df.Book<ULong64_t>(std::move(helperNoUnderflowOverflow), {"rdfentry_"});

  ASSERT_EQ(h1->GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(h1->GetNumDimensions(), 1);
  ASSERT_EQ(h1NoUnderflowOverflow->GetTotalNumBins(), Bins);
  EXPECT_EQ(h1NoUnderflowOverflow->GetNumDimensions(), 1);

  EXPECT_EQ(h1->GetBinContent(0), 0);
  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1->GetBinContent(i + 1), 1);
    EXPECT_EQ(h1NoUnderflowOverflow->GetBinContent(i), 1);
  }
  EXPECT_EQ(h1->GetBinContent(Bins + 1), 0);
}

TEST(EPHistFillAddHelper, IntRegular2D) {
  // The same histogram as IntRegular2D.FillOnlyInner in regular.cxx
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);

  auto df = ROOT::RDataFrame(BinsX * BinsY)
                .Define("x", [&](ULong64_t entry) { return entry / BinsY; },
                        {"rdfentry_"})
                .Define("y", [&](ULong64_t entry) { return entry % BinsY; },
                        {"rdfentry_"});

  // TODO: Ideally accept a brace-enclosed initializer list
  std::vector<EPHist::AxisVariant> axes{axisX, axisY};
  EPHist::Util::EPHistFillAddHelper<int> helper(df.GetNSlots(), axes);
  auto h2 = df.Book<ULong64_t, ULong64_t>(std::move(helper), {"x", "y"});

  EXPECT_EQ(h2->GetTotalNumBins(), (BinsX + 2) * (BinsY + 2));
  EXPECT_EQ(h2->GetNumDimensions(), 2);

  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent(y), 0);
  }
  for (std::size_t x = 0; x < BinsX; x++) {
    EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2)), 0);
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2) + y + 1), 1);
    }
    EXPECT_EQ(h2->GetBinContent((x + 2) * (BinsY + 2) - 1), 0);
  }
  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
  }
}

TEST(EPHistFillAtomicHelper, IntRegular1D) {
  // The same histograms as IntRegular1D.FillOnlyInner in regular.cxx
  static constexpr std::size_t Bins = 20;
  ROOT::RDataFrame df(Bins);

  EPHist::Util::EPHistFillAtomicHelper<int> helper(Bins, 0, Bins);
  auto h1 = df.Book<ULong64_t>(std::move(helper), {"rdfentry_"});

  EPHist::RegularAxis axisNoUnderflowOverflow(
      Bins, 0, Bins, /*enableUnderflowOverflowBins=*/false);
  EPHist::Util::EPHistFillAtomicHelper<int> helperNoUnderflowOverflow(
      axisNoUnderflowOverflow);
  auto h1NoUnderflowOverflow =
      df.Book<ULong64_t>(std::move(helperNoUnderflowOverflow), {"rdfentry_"});

  ASSERT_EQ(h1->GetTotalNumBins(), Bins + 2);
  EXPECT_EQ(h1->GetNumDimensions(), 1);
  ASSERT_EQ(h1NoUnderflowOverflow->GetTotalNumBins(), Bins);
  EXPECT_EQ(h1NoUnderflowOverflow->GetNumDimensions(), 1);

  EXPECT_EQ(h1->GetBinContent(0), 0);
  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1->GetBinContent(i + 1), 1);
    EXPECT_EQ(h1NoUnderflowOverflow->GetBinContent(i), 1);
  }
  EXPECT_EQ(h1->GetBinContent(Bins + 1), 0);
}

TEST(EPHistFillAtomicHelper, IntRegular2D) {
  // The same histogram as IntRegular2D.FillOnlyInner in regular.cxx
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);

  auto df = ROOT::RDataFrame(BinsX * BinsY)
                .Define("x", [&](ULong64_t entry) { return entry / BinsY; },
                        {"rdfentry_"})
                .Define("y", [&](ULong64_t entry) { return entry % BinsY; },
                        {"rdfentry_"});

  // TODO: Ideally accept a brace-enclosed initializer list
  std::vector<EPHist::AxisVariant> axes{axisX, axisY};
  EPHist::Util::EPHistFillAtomicHelper<int> helper(axes);
  auto h2 = df.Book<ULong64_t, ULong64_t>(std::move(helper), {"x", "y"});

  EXPECT_EQ(h2->GetTotalNumBins(), (BinsX + 2) * (BinsY + 2));
  EXPECT_EQ(h2->GetNumDimensions(), 2);

  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent(y), 0);
  }
  for (std::size_t x = 0; x < BinsX; x++) {
    EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2)), 0);
    for (std::size_t y = 0; y < BinsY; y++) {
      EXPECT_EQ(h2->GetBinContent((x + 1) * (BinsY + 2) + y + 1), 1);
    }
    EXPECT_EQ(h2->GetBinContent((x + 2) * (BinsY + 2) - 1), 0);
  }
  for (std::size_t y = 0; y < BinsY + 2; y++) {
    EXPECT_EQ(h2->GetBinContent((BinsX + 1) * (BinsY + 2) + y), 0);
  }
}
