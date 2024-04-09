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
  std::vector<double> bins;
  std::size_t binIndex = -1;
  if (const auto *regular = std::get_if<RegularAxis>(&axes[0])) {
    binIndex = regular->GetFirstBin();
    for (std::size_t i = 0; i < regular->GetNumBins(); i++) {
      bins.push_back(regular->ComputeLowEdge(i));
    }
    bins.push_back(regular->GetHigh());
  } else if (const auto *variable = std::get_if<VariableBinAxis>(&axes[0])) {
    binIndex = variable->GetFirstBin();
    bins = variable->GetBins();
  }

  for (std::size_t i = 0; i < bins.size() - 1; i++) {
    os << bins[i] << " ";
    h.PrintBinContent(binIndex, os);
    os << "\n";
    binIndex++;
  }
  os << bins.back() << " ";
  h.PrintBinContent(binIndex - 1, os);
  os << "\n";
}
