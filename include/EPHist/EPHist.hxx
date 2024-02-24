// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include <cassert>
#include <cstddef>
#include <memory>

namespace EPHist {

template <typename T> class EPHist final {
  std::unique_ptr<T[]> fData;
  std::size_t fNumBins;

  double fLow;
  double fHigh;
  double fInvBinWidth;

public:
  EPHist(std::size_t numBins, double low, double high)
      : fData(new T[numBins]{}), fNumBins(numBins), fLow(low), fHigh(high) {
    fInvBinWidth = numBins / (high - low);
  }

  EPHist(const EPHist<T> &) = delete;
  EPHist(EPHist<T> &&) = default;
  EPHist<T> &operator=(const EPHist<T> &) = delete;
  EPHist<T> &operator=(EPHist<T> &&) = default;
  ~EPHist() = default;

  void Clear() {
    for (std::size_t i = 0; i < fNumBins; i++) {
      fData[i] = {};
    }
  }

  const T &GetBinContent(std::size_t bin) const {
    assert(bin >= 0 && bin < fNumBins);
    return fData[bin];
  }
  std::size_t GetNumBins() const { return fNumBins; }

  void Fill(double x) {
    std::size_t bin = (x - fLow) * fInvBinWidth;
    assert(bin >= 0 && bin < fNumBins);
    fData[bin]++;
  }
};

} // namespace EPHist

#endif
