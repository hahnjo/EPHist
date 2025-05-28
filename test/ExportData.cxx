// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/Util/ExportData.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>

TEST(ExportTextData, IntRegular1D) {
  // Attempt to reproduce the example given in the PGFPlots manual:
  EPHist::EPHist<int> h1(3, 1, 10);
  EPHist::RegularAxis axisNoFlowBins(3, 1, 10, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (double x : {1, 2, 1, 5, 4, 10, 7, 10, 9, 8, 9, 9}) {
    h1.Fill(x);
    h1NoFlowBins.Fill(x);
  }

  const char *Expected = R"(1 3
4 2
7 5
10 5
)";

  std::stringstream ss;
  EPHist::Util::ExportTextData(h1, ss);
  EXPECT_EQ(ss.str(), Expected);

  ss.str("");
  EPHist::Util::ExportTextData(h1NoFlowBins, ss);
  EXPECT_EQ(ss.str(), Expected);
}

TEST(ExportTextData, IntVariable1D) {
  // Reproduce the example given in the PGFPlots manual:
  std::vector<double> bins = {1, 4, 7, 10 + 1e-9};
  EPHist::VariableBinAxis axis(bins);
  EPHist::EPHist<int> h1(axis);
  EPHist::VariableBinAxis axisNoFlowBins(bins, /*enableFlowBins=*/false);
  EPHist::EPHist<int> h1NoFlowBins(axisNoFlowBins);

  for (double x : {1, 2, 1, 5, 4, 10, 7, 10, 9, 8, 9, 9}) {
    h1.Fill(x);
    h1NoFlowBins.Fill(x);
  }

  const char *Expected = R"(1 3
4 2
7 7
10 7
)";

  std::stringstream ss;
  EPHist::Util::ExportTextData(h1, ss);
  EXPECT_EQ(ss.str(), Expected);

  ss.str("");
  EPHist::Util::ExportTextData(h1NoFlowBins, ss);
  EXPECT_EQ(ss.str(), Expected);
}

TEST(ExportTextData, DoubleRegular1D) {
  // Attempt to reproduce the example given in the PGFPlots manual:
  EPHist::EPHist<double> h1(3, 1, 10);
  EPHist::RegularAxis axisNoFlowBins(3, 1, 10, /*enableFlowBins=*/false);
  EPHist::EPHist<double> h1NoFlowBins(axisNoFlowBins);

  for (double x : {1, 2, 1, 5, 4, 10, 7, 10, 9, 8, 9, 9}) {
    h1.Fill(x);
    h1NoFlowBins.Fill(x);
  }

  const char *Expected = R"(1 3
4 2
7 5
10 5
)";

  std::stringstream ss;
  EPHist::Util::ExportTextData(h1, ss);
  EXPECT_EQ(ss.str(), Expected);

  ss.str("");
  EPHist::Util::ExportTextData(h1NoFlowBins, ss);
  EXPECT_EQ(ss.str(), Expected);
}

TEST(ExportTextData, IntRegular2D) {
  EPHist::RegularAxis axis(20, 0.0, 1.0);
  EPHist::EPHist<int> h2({axis, axis});

  std::stringstream ss;
  EXPECT_THROW(EPHist::Util::ExportTextData(h2, ss), std::invalid_argument);
}
