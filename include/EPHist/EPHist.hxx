// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include "RegularAxis.hxx"
#include "VariableBinAxis.hxx"

#include <cassert>
#include <cstddef>
#include <memory>
#include <tuple>
#include <variant>
#include <vector>

namespace EPHist {

using AxisVariant = std::variant<RegularAxis, VariableBinAxis>;

template <typename T> class EPHist final {
  std::unique_ptr<T[]> fData;
  std::size_t fNumBins;

  std::vector<AxisVariant> fAxes;

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
  explicit EPHist(const VariableBinAxis &axis)
      : fData(new T[axis.GetNumBins()]{}), fNumBins(axis.GetNumBins()),
        fAxes({axis}) {}

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

  const std::vector<AxisVariant> &GetAxes() const { return fAxes; }
  std::size_t GetNumDimensions() const { return fAxes.size(); }

private:
  template <std::size_t I, typename... A>
  std::size_t ComputeBin(std::size_t bin, const std::tuple<A...> &args) const {
    if (const auto *regular = std::get_if<RegularAxis>(&fAxes[I])) {
      bin *= regular->GetNumBins();
      bin += regular->ComputeBin(std::get<I>(args));
    } else if (const auto *variable = std::get_if<VariableBinAxis>(&fAxes[I])) {
      bin *= variable->GetNumBins();
      bin += variable->ComputeBin(std::get<I>(args));
    }
    if constexpr (I + 1 < sizeof...(A)) {
      return ComputeBin<I + 1>(bin, args);
    }
    return bin;
  }

  template <std::size_t I, class Axis, class... Axes>
  std::size_t ComputeBin(std::size_t bin,
                         const typename Axis::ArgumentType &arg,
                         const typename Axes::ArgumentType &...args) const {
    const auto &axis = std::get<Axis>(fAxes[I]);
    bin = bin * axis.GetNumBins() + axis.ComputeBin(arg);
    if constexpr (sizeof...(Axes) > 0) {
      return ComputeBin<I + 1, Axes...>(bin, args...);
    }
    return bin;
  }

public:
  template <typename... A> void Fill(const std::tuple<A...> &args) {
    std::size_t bin = ComputeBin<0>(0, args);
    fData[bin]++;
  }

  template <typename... A> void Fill(const A &...args) {
    Fill(std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args) {
    std::size_t bin = ComputeBin<0, Axes...>(0, args...);
    fData[bin]++;
  }
};

} // namespace EPHist

#endif
