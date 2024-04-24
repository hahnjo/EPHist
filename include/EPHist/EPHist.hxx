// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include "Atomic.hxx"
#include "Axes.hxx"
#include "DoubleBinWithError.hxx"
#include "Weight.hxx"

#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <vector>

namespace EPHist {

template <typename T> class EPHist final {
  std::unique_ptr<T[]> fData;
  std::size_t fTotalNumBins;

  Detail::Axes fAxes;

public:
  explicit EPHist(const std::vector<AxisVariant> &axes) : fAxes(axes) {
    fTotalNumBins = fAxes.ComputeTotalNumBins();
    fData.reset(new T[fTotalNumBins]{});
  }

  EPHist(std::size_t numBins, double low, double high)
      : EPHist({RegularAxis(numBins, low, high)}) {}
  explicit EPHist(const RegularAxis &axis)
      : EPHist(std::vector<AxisVariant>{axis}) {}
  explicit EPHist(const VariableBinAxis &axis)
      : EPHist(std::vector<AxisVariant>{axis}) {}

  // Copy constructor and assignment operator are deleted to avoid surprises.
  // Use the explicit Clone() function to create a copy.
  EPHist(const EPHist<T> &) = delete;
  EPHist(EPHist<T> &&) = default;
  EPHist<T> &operator=(const EPHist<T> &) = delete;
  EPHist<T> &operator=(EPHist<T> &&) = default;
  ~EPHist() = default;

  void Add(const EPHist<T> &other) {
    if (fAxes != other.fAxes) {
      throw std::invalid_argument("axes configuration not identical");
    }
    for (std::size_t i = 0; i < fTotalNumBins; i++) {
      fData[i] += other.fData[i];
    }
  }

  void AddAtomic(const EPHist<T> &other) {
    if (fAxes != other.fAxes) {
      throw std::invalid_argument("axes configuration not identical");
    }
    for (std::size_t i = 0; i < fTotalNumBins; i++) {
      Internal::AtomicAdd(&fData[i], other.fData[i]);
    }
  }

  void Clear() {
    for (std::size_t i = 0; i < fTotalNumBins; i++) {
      fData[i] = {};
    }
  }

  EPHist<T> Clone() {
    EPHist<T> h(fAxes.GetVector());
    for (std::size_t i = 0; i < fTotalNumBins; i++) {
      h.fData[i] += fData[i];
    }
    return h;
  }

  const T &GetBinContent(std::size_t bin) const {
    assert(bin >= 0 && bin < fTotalNumBins);
    return fData[bin];
  }
  std::size_t GetTotalNumBins() const { return fTotalNumBins; }

  const std::vector<AxisVariant> &GetAxes() const { return fAxes.GetVector(); }
  std::size_t GetNumDimensions() const { return fAxes.GetNumDimensions(); }

public:
  template <typename... A> void Fill(const std::tuple<A...> &args) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin(args);
    if (bin.second) {
      fData[bin.first]++;
    }
  }

  template <typename... A> void Fill(const A &...args) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args) {
    if (sizeof...(Axes) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin<Axes...>(args...);
    if (bin.second) {
      fData[bin.first]++;
    }
  }

  static constexpr bool SupportsWeightedFill =
      std::is_floating_point_v<T> || std::is_same_v<T, DoubleBinWithError>;

  template <typename... A> void Fill(Weight w, const std::tuple<A...> &args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin(args);
    if (bin.second) {
      fData[bin.first] += w.fValue;
    }
  }

  template <typename... A> void Fill(Weight w, const A &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(w, std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(Weight w, const typename Axes::ArgumentType &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(Axes) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin<Axes...>(args...);
    if (bin.second) {
      fData[bin.first] += w.fValue;
    }
  }

  template <typename... A> void FillAtomic(const std::tuple<A...> &args) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin(args);
    if (bin.second) {
      Internal::AtomicInc(&fData[bin.first]);
    }
  }

  template <typename... A> void FillAtomic(const A &...args) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillAtomic(std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void FillAtomic(const typename Axes::ArgumentType &...args) {
    if (sizeof...(Axes) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin<Axes...>(args...);
    if (bin.second) {
      Internal::AtomicInc(&fData[bin.first]);
    }
  }

  template <typename... A>
  void FillAtomic(Weight w, const std::tuple<A...> &args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin(args);
    if (bin.second) {
      Internal::AtomicAddDouble(&fData[bin.first], w.fValue);
    }
  }

  template <typename... A> void FillAtomic(Weight w, const A &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillAtomic(w, std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void FillAtomic(Weight w, const typename Axes::ArgumentType &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(Axes) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    auto bin = fAxes.ComputeBin<Axes...>(args...);
    if (bin.second) {
      Internal::AtomicAddDouble(&fData[bin.first], w.fValue);
    }
  }
};

} // namespace EPHist

#endif
