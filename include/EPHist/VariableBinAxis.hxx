// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_VARIABLEBINAXIS
#define EPHIST_VARIABLEBINAXIS

#include <cassert>
#include <cstddef>
#include <vector>

namespace EPHist {

class alignas(8) VariableBinAxis final {
public:
  using ArgumentType = double;

private:
  std::vector<double> fBins;

public:
  explicit VariableBinAxis(const std::vector<double> &bins) : fBins(bins) {}

  std::size_t GetNumBins() const { return fBins.size() - 1; }
  double GetBin(std::size_t bin) const { return fBins[bin]; }

  std::size_t ComputeBin(double x) const {
    // FIXME: Optimize with binary search...
    assert(x >= fBins.front());
    for (std::size_t bin = 0; bin < fBins.size() - 1; bin++) {
      if (x < fBins[bin + 1]) {
        return bin;
      }
    }
    assert(x < fBins.back());
    return fBins.size() - 1;
  }

  friend bool operator==(const VariableBinAxis &lhs,
                         const VariableBinAxis &rhs) {
    return lhs.fBins == rhs.fBins;
  }
};

} // namespace EPHist

#endif
