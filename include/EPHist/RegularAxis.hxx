// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_REGULARAXIS
#define EPHIST_REGULARAXIS

#include <cassert>
#include <cstddef>

namespace EPHist {

class alignas(8) RegularAxis final {
public:
  using ArgumentType = double;

private:
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

  friend bool operator==(const RegularAxis &lhs, const RegularAxis &rhs) {
    return lhs.fNumBins == rhs.fNumBins && lhs.fLow == rhs.fLow &&
           lhs.fHigh == rhs.fHigh;
  }
};

} // namespace EPHist

#endif
