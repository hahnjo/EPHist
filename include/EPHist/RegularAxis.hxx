// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_REGULARAXIS
#define EPHIST_REGULARAXIS

#include "BinIndex.hxx"
#include "BinIndexRange.hxx"

#include <cassert>
#include <cstddef>
#include <utility>

namespace EPHist {

class RegularAxis final {
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
  bool AreUnderflowOverflowBinsEnabled() const {
    return fEnableUnderflowOverflowBins;
  }

  double ComputeLowEdge(std::size_t bin) const {
    assert(0 <= bin && bin < fNumBins);
    return fLow + bin * (fHigh - fLow) / fNumBins;
  }

  double ComputeHighEdge(std::size_t bin) const {
    assert(0 <= bin && bin < fNumBins);
    return fLow + (bin + 1) * (fHigh - fLow) / fNumBins;
  }

  std::pair<std::size_t, bool> GetBin(BinIndex index) const {
    if (index.IsUnderflow()) {
      return {fNumBins, fEnableUnderflowOverflowBins};
    } else if (index.IsOverflow()) {
      return {fNumBins + 1, fEnableUnderflowOverflowBins};
    } else if (index.IsInvalid()) {
      return {0, false};
    }
    assert(index.IsNormal());
    std::size_t bin = index.GetIndex();
    return {bin, bin < fNumBins};
  }

  std::pair<std::size_t, bool> ComputeBin(double x) const {
    bool underflow = x < fLow;
    // Put NaNs into overflow bin.
    bool overflow = !(x < fHigh);
    if (underflow) {
      return {fNumBins, fEnableUnderflowOverflowBins};
    } else if (overflow) {
      return {fNumBins + 1, fEnableUnderflowOverflowBins};
    }

    std::size_t bin = (x - fLow) * fInvBinWidth;
    return {bin, true};
  }

  RegularAxis Slice(const BinIndexRange &range) const {
    const auto normalRange = range.GetNormalRange(fNumBins);

    const auto begin = normalRange.GetBegin();
    const auto end = normalRange.GetEnd();
    assert(begin.IsNormal());
    assert(end.IsNormal());
    assert(begin <= end);

    const auto numBins = end.GetIndex() - begin.GetIndex();
    const auto low = ComputeLowEdge(begin.GetIndex());
    const auto high = ComputeHighEdge(end.GetIndex() - 1);
    // Always enable underflow and overflow bins.
    const auto enableUnderflowOverflowBins = true;
    return RegularAxis(numBins, low, high, enableUnderflowOverflowBins);
  }

  friend bool operator==(const RegularAxis &lhs, const RegularAxis &rhs) {
    return lhs.fNumBins == rhs.fNumBins && lhs.fLow == rhs.fLow &&
           lhs.fHigh == rhs.fHigh &&
           lhs.fEnableUnderflowOverflowBins == rhs.fEnableUnderflowOverflowBins;
  }
};

} // namespace EPHist

#endif
