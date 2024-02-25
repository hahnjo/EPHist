// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_REGULARAXIS
#define EPHIST_REGULARAXIS

#include <cassert>
#include <cstddef>

namespace EPHist {

class RegularAxis final {
  std::size_t fNumBins;
  double fLow;
  double fHigh;
  double fInvBinWidth;

public:
  RegularAxis(std::size_t numBins, double low, double high)
      : fNumBins(numBins), fLow(low), fHigh(high) {
    fInvBinWidth = numBins / (high - low);
  }

  std::size_t GetNumBins() const { return fNumBins; }
  double GetLow() const { return fLow; }
  double GetHigh() const { return fHigh; }

  std::size_t ComputeBin(double x) const {
    std::size_t bin = (x - fLow) * fInvBinWidth;
    assert(bin >= 0 && bin < fNumBins);
    return bin;
  }
};

} // namespace EPHist

#endif