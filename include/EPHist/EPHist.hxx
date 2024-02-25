// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include "RegularAxis.hxx"

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

namespace EPHist {

template <typename T> class EPHist final {
  std::unique_ptr<T[]> fData;
  std::size_t fNumBins;

  std::vector<RegularAxis> fAxes;

public:
  EPHist(std::size_t numBins, double low, double high)
      : fData(new T[numBins]{}), fNumBins(numBins),
        fAxes({RegularAxis(numBins, low, high)}) {}
  explicit EPHist(const RegularAxis &axis)
      : fData(new T[axis.GetNumBins()]{}), fNumBins(axis.GetNumBins()),
        fAxes({axis}) {}
  explicit EPHist(const std::vector<RegularAxis> &axes) {
    fNumBins = 1;
    for (auto &&axis : axes) {
      fAxes.push_back(axis);
      fNumBins *= axis.GetNumBins();
    }
    fData.reset(new T[fNumBins]{});
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
  std::size_t GetNumDimensions() const { return fAxes.size(); }

  void Fill(double x) {
    std::size_t bin = fAxes[0].ComputeBin(x);
    fData[bin]++;
  }
  void Fill(double x, double y) {
    std::size_t bin = fAxes[0].ComputeBin(x);
    bin *= fAxes[1].GetNumBins();
    bin += fAxes[1].ComputeBin(y);
    fData[bin]++;
  }
  void Fill(double x, double y, double z) {
    std::size_t bin = fAxes[0].ComputeBin(x);
    bin *= fAxes[1].GetNumBins();
    bin += fAxes[1].ComputeBin(y);
    bin *= fAxes[2].GetNumBins();
    bin += fAxes[2].ComputeBin(z);
    fData[bin]++;
  }
};

} // namespace EPHist

#endif
