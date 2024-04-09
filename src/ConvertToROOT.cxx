// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/Util/ConvertToROOT.hxx>

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/VariableBinAxis.hxx>

#include <ostream>
#include <stdexcept>
#include <variant>

namespace {
template <typename Hist, typename T>
std::unique_ptr<Hist> ConvertToTH1(const EPHist::EPHist<T> &h) {
  if (h.GetNumDimensions() != 1) {
    throw std::invalid_argument("TH1 requires one dimension");
  }

  std::unique_ptr<Hist> out;

  const auto &axes = h.GetAxes();
  std::size_t binIndex = -1;
  std::size_t numBins = -1;
  if (const auto *regular = std::get_if<EPHist::RegularAxis>(&axes[0])) {
    binIndex = regular->GetFirstBin();
    numBins = regular->GetNumBins();
    out.reset(new Hist("", "", numBins, regular->GetLow(), regular->GetHigh()));
  } else if (const auto *variable =
                 std::get_if<EPHist::VariableBinAxis>(&axes[0])) {
    binIndex = variable->GetFirstBin();
    numBins = variable->GetNumBins();
    const auto &bins = variable->GetBins();
    out.reset(new Hist("", "", numBins, bins.data()));
  }

  if (binIndex != 0) {
    // Set underflow and overflow bins.
    out->SetBinContent(0, h.GetBinContent(0));
    out->SetBinContent(numBins + 1, h.GetBinContent(numBins + 1));
  }

  for (std::size_t i = 0; i < numBins; i++) {
    out->SetBinContent(i + 1, h.GetBinContent(binIndex));
    binIndex++;
  }
  return out;
}
} // namespace

std::unique_ptr<TH1I> EPHist::Util::ConvertToTH1I(const EPHist<int> &h) {
  return ConvertToTH1<TH1I>(h);
}

std::unique_ptr<TH1F> EPHist::Util::ConvertToTH1F(const EPHist<float> &h) {
  return ConvertToTH1<TH1F>(h);
}

std::unique_ptr<TH1D> EPHist::Util::ConvertToTH1D(const EPHist<double> &h) {
  return ConvertToTH1<TH1D>(h);
}
