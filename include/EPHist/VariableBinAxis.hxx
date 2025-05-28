// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_VARIABLEBINAXIS
#define EPHIST_VARIABLEBINAXIS

#include "BinIndex.hxx"

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

namespace EPHist {

class VariableBinAxis final {
public:
  using ArgumentType = double;

private:
  std::vector<double> fBinEdges;
  bool fEnableFlowBins;

public:
  explicit VariableBinAxis(std::vector<double> binEdges,
                           bool enableFlowBins = true)
      : fBinEdges(std::move(binEdges)), fEnableFlowBins(enableFlowBins) {}

  std::size_t GetNumBins() const { return fBinEdges.size() - 1; }
  std::size_t GetTotalNumBins() const {
    return fEnableFlowBins ? fBinEdges.size() + 1 : fBinEdges.size() - 1;
  }
  const std::vector<double> &GetBinEdges() const { return fBinEdges; }
  double GetBinEdge(std::size_t bin) const { return fBinEdges[bin]; }
  bool AreFlowBinsEnabled() const { return fEnableFlowBins; }

  std::pair<std::size_t, bool> GetBin(BinIndex index) const {
    if (index.IsUnderflow()) {
      return {fBinEdges.size() - 1, fEnableFlowBins};
    } else if (index.IsOverflow()) {
      return {fBinEdges.size(), fEnableFlowBins};
    } else if (index.IsInvalid()) {
      return {0, false};
    }
    assert(index.IsNormal());
    std::size_t bin = index.GetIndex();
    return {bin, bin < fBinEdges.size() - 1};
  }

  std::pair<std::size_t, bool> ComputeBin(double x) const {
    if (x < fBinEdges.front()) {
      return {fBinEdges.size() - 1, fEnableFlowBins};
    } else if (!(x < fBinEdges.back())) {
      // Put NaNs into overflow bin.
      return {fBinEdges.size(), fEnableFlowBins};
    }

    // FIXME: Optimize with binary search...
    assert(x >= fBinEdges.front());
    for (std::size_t bin = 0; bin < fBinEdges.size() - 2; bin++) {
      if (x < fBinEdges[bin + 1]) {
        return {bin, true};
      }
    }
    assert(x < fBinEdges.back());
    std::size_t bin = fBinEdges.size() - 2;
    return {bin, true};
  }

  VariableBinAxis Slice(const BinIndexRange &range) const {
    const auto normalRange = range.GetNormalRange(fBinEdges.size() - 1);

    const auto begin = normalRange.GetBegin();
    const auto end = normalRange.GetEnd();
    assert(begin.IsNormal());
    assert(end.IsNormal());
    assert(begin <= end);

    std::vector binEdges(fBinEdges.begin() + begin.GetIndex(),
                         fBinEdges.begin() + end.GetIndex() + 1);
    // Always enable underflow and overflow bins.
    const auto enableFlowBins = true;
    return VariableBinAxis(std::move(binEdges), enableFlowBins);
  }

  friend bool operator==(const VariableBinAxis &lhs,
                         const VariableBinAxis &rhs) {
    return lhs.fBinEdges == rhs.fBinEdges &&
           lhs.fEnableFlowBins == rhs.fEnableFlowBins;
  }
};

} // namespace EPHist

#endif
