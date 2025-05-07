// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/Util/ConvertToROOT.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

#include <stdexcept>

TEST(ConvertToTH1I, IntRegular1D) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EPHist::RegularAxis axisNoUnderflowOverflow(
      Bins, 0, Bins, /*enableUnderflowOverflowBins=*/false);
  EPHist::EPHist<int> h1NoUnderflowOverflow(axisNoUnderflowOverflow);

  auto fill = [&](double x) {
    h1.Fill(x);
    h1NoUnderflowOverflow.Fill(x);
  };

  fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    fill(i);
  }
  fill(100);

  auto th1i = EPHist::Util::ConvertToTH1I(h1);
  ASSERT_EQ(th1i->GetNbinsX(), Bins);
  ASSERT_EQ(th1i->GetNbinsY(), 1);
  ASSERT_EQ(th1i->GetNbinsZ(), 1);

  TAxis *xAxis = th1i->GetXaxis();
  EXPECT_FALSE(xAxis->IsVariableBinSize());
  EXPECT_EQ(xAxis->GetNbins(), Bins);
  EXPECT_EQ(xAxis->GetXmin(), 0.0);
  EXPECT_EQ(xAxis->GetXmax(), Bins);

  for (std::size_t i = 0; i < Bins + 2; i++) {
    EXPECT_EQ(th1i->GetBinContent(i), 1);
  }

  th1i = EPHist::Util::ConvertToTH1I(h1NoUnderflowOverflow);
  ASSERT_EQ(th1i->GetNbinsX(), Bins);
  ASSERT_EQ(th1i->GetNbinsY(), 1);
  ASSERT_EQ(th1i->GetNbinsZ(), 1);

  xAxis = th1i->GetXaxis();
  EXPECT_FALSE(xAxis->IsVariableBinSize());
  EXPECT_EQ(xAxis->GetNbins(), Bins);
  EXPECT_EQ(xAxis->GetXmin(), 0.0);
  EXPECT_EQ(xAxis->GetXmax(), Bins);

  EXPECT_EQ(th1i->GetBinContent(0), 0);
  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(th1i->GetBinContent(i + 1), 1);
  }
  EXPECT_EQ(th1i->GetBinContent(Bins + 1), 0);
}

TEST(ConvertToTH1I, IntVariable1D) {
  static constexpr std::size_t Bins = 20;
  std::vector<double> bins;
  for (std::size_t i = 0; i < Bins; i++) {
    bins.push_back(i);
  }
  bins.push_back(Bins);

  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);

  h1.Fill(-100);
  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }
  h1.Fill(100);

  auto th1i = EPHist::Util::ConvertToTH1I(h1);
  ASSERT_EQ(th1i->GetNbinsX(), Bins);
  ASSERT_EQ(th1i->GetNbinsY(), 1);
  ASSERT_EQ(th1i->GetNbinsZ(), 1);

  TAxis *xAxis = th1i->GetXaxis();
  EXPECT_TRUE(xAxis->IsVariableBinSize());
  EXPECT_EQ(xAxis->GetNbins(), Bins);
  EXPECT_EQ(xAxis->GetXmin(), 0.0);
  EXPECT_EQ(xAxis->GetXmax(), Bins);

  const TArrayD *xBins = xAxis->GetXbins();
  ASSERT_EQ(xBins->GetSize(), bins.size());
  for (std::size_t i = 0; i < bins.size(); i++) {
    // Strong equality, there should be no floating point rounding!
    EXPECT_EQ(xBins->At(i), bins[i]);
  }

  for (std::size_t i = 0; i < Bins + 2; i++) {
    EXPECT_EQ(th1i->GetBinContent(i), 1);
  }
}

TEST(ConvertToTH1I, IntRegular2D) {
  EPHist::RegularAxis axis(20, 0.0, 1.0);
  EPHist::EPHist<int> h2({axis, axis});

  EXPECT_THROW(EPHist::Util::ConvertToTH1I(h2), std::invalid_argument);
}
