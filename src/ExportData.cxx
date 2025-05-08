// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/Util/ExportData.hxx>

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <ostream>
#include <stdexcept>
#include <variant>

void EPHist::Util::ExportTextData(const EPHistForExport &h, std::ostream &os) {
  if (h.GetNumDimensions() != 1) {
    throw std::invalid_argument("only one dimension supported");
  }

  const auto &axes = h.GetAxes();
  std::vector<double> binEdges;
  if (const auto *regular = std::get_if<RegularAxis>(&axes[0])) {
    for (std::size_t i = 0; i < regular->GetNumBins(); i++) {
      binEdges.push_back(regular->ComputeLowEdge(i));
    }
    binEdges.push_back(regular->GetHigh());
  } else if (const auto *variable = std::get_if<VariableBinAxis>(&axes[0])) {
    binEdges = variable->GetBinEdges();
  }

  for (std::size_t i = 0; i < binEdges.size() - 1; i++) {
    os << binEdges[i] << " ";
    h.PrintBinContent(i, os);
    os << "\n";
  }
  os << binEdges.back() << " ";
  h.PrintBinContent(binEdges.size() - 2, os);
  os << "\n";
}
