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
  std::vector<double> fBins;
  bool fEnableUnderflowOverflowBins;

public:
  explicit VariableBinAxis(const std::vector<double> &bins,
                           bool enableUnderflowOverflowBins = true)
      : fBins(bins), fEnableUnderflowOverflowBins(enableUnderflowOverflowBins) {
  }

  std::size_t GetNumBins() const { return fBins.size() - 1; }
  std::size_t GetTotalNumBins() const {
    return fEnableUnderflowOverflowBins ? fBins.size() + 1 : fBins.size() - 1;
  }
  const std::vector<double> &GetBins() const { return fBins; }
  double GetBin(std::size_t bin) const { return fBins[bin]; }

  std::size_t GetFirstBin() const {
    return fEnableUnderflowOverflowBins ? 1 : 0;
  }

  std::pair<std::size_t, bool> ComputeBin(double x) const {
    if (x < fBins.front()) {
      return {0, fEnableUnderflowOverflowBins};
    } else if (!(x < fBins.back())) {
      // Put NaNs into overflow bin.
      return {fBins.size(), fEnableUnderflowOverflowBins};
    }

    // FIXME: Optimize with binary search...
    assert(x >= fBins.front());
    for (std::size_t bin = 0; bin < fBins.size() - 1; bin++) {
      if (x < fBins[bin + 1]) {
        // If underflow and overflow bins are enabled, shift bin by one.
        bin += fEnableUnderflowOverflowBins;
        return {bin, true};
      }
    }
    assert(x < fBins.back());
    std::size_t bin = fBins.size() - 1;
    // If underflow and overflow bins are enabled, shift bin by one.
    bin += fEnableUnderflowOverflowBins;
    return {bin, fEnableUnderflowOverflowBins};
  }

  friend bool operator==(const VariableBinAxis &lhs,
                         const VariableBinAxis &rhs) {
    return lhs.fBins == rhs.fBins &&
           lhs.fEnableUnderflowOverflowBins == rhs.fEnableUnderflowOverflowBins;
  }
};

} // namespace EPHist

#endif
