// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_VARIABLEBINAXIS
#define EPHIST_VARIABLEBINAXIS

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
  bool fEnableUnderflowOverflowBins;

public:
  explicit VariableBinAxis(const std::vector<double> &binEdges,
                           bool enableUnderflowOverflowBins = true)
      : fBinEdges(binEdges),
        fEnableUnderflowOverflowBins(enableUnderflowOverflowBins) {}

  std::size_t GetNumBins() const { return fBinEdges.size() - 1; }
  std::size_t GetTotalNumBins() const {
    return fEnableUnderflowOverflowBins ? fBinEdges.size() + 1
                                        : fBinEdges.size() - 1;
  }
  const std::vector<double> &GetBinEdges() const { return fBinEdges; }
  double GetBinEdge(std::size_t bin) const { return fBinEdges[bin]; }
  bool AreUnderflowOverflowBinsEnabled() const {
    return fEnableUnderflowOverflowBins;
  }

  std::pair<std::size_t, bool> ComputeBin(double x) const {
    if (x < fBinEdges.front()) {
      return {fBinEdges.size() - 1, fEnableUnderflowOverflowBins};
    } else if (!(x < fBinEdges.back())) {
      // Put NaNs into overflow bin.
      return {fBinEdges.size(), fEnableUnderflowOverflowBins};
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

  friend bool operator==(const VariableBinAxis &lhs,
                         const VariableBinAxis &rhs) {
    return lhs.fBinEdges == rhs.fBinEdges &&
           lhs.fEnableUnderflowOverflowBins == rhs.fEnableUnderflowOverflowBins;
  }
};

} // namespace EPHist

#endif
