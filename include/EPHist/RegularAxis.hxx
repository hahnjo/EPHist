// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_REGULARAXIS
#define EPHIST_REGULARAXIS

#include <cassert>
#include <cstddef>
#include <utility>

namespace EPHist {

class alignas(8) RegularAxis final {
public:
  using ArgumentType = double;

private:
  std::size_t fNumBins;
  double fLow;
  double fHigh;
  double fInvBinWidth;
  bool fEnableUnderflowOverflowBins;

public:
  RegularAxis(std::size_t numBins, double low, double high,
              bool enableUnderflowOverflowBins = true)
      : fNumBins(numBins), fLow(low), fHigh(high),
        fEnableUnderflowOverflowBins(enableUnderflowOverflowBins) {
    fInvBinWidth = numBins / (high - low);
  }

  std::size_t GetNumBins() const { return fNumBins; }
  std::size_t GetTotalNumBins() const {
    return fEnableUnderflowOverflowBins ? fNumBins + 2 : fNumBins;
  }
  double GetLow() const { return fLow; }
  double GetHigh() const { return fHigh; }

  std::pair<std::size_t, bool> ComputeBin(double x) const {
    bool underflow = x < fLow;
    // Put NaNs into overflow bin.
    bool overflow = !(x < fHigh);
    if (underflow) {
      return {0, fEnableUnderflowOverflowBins};
    } else if (overflow) {
      return {fNumBins + 1, fEnableUnderflowOverflowBins};
    }

    std::size_t bin = (x - fLow) * fInvBinWidth;
    // If underflow and overflow bins are enabled, shift bin by one.
    bin += fEnableUnderflowOverflowBins;
    return {bin, true};
  }

  friend bool operator==(const RegularAxis &lhs, const RegularAxis &rhs) {
    return lhs.fNumBins == rhs.fNumBins && lhs.fLow == rhs.fLow &&
           lhs.fHigh == rhs.fHigh &&
           lhs.fEnableUnderflowOverflowBins == rhs.fEnableUnderflowOverflowBins;
  }
};

} // namespace EPHist

#endif
