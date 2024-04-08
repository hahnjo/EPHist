// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include "RegularAxis.hxx"
#include "VariableBinAxis.hxx"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <variant>
#include <vector>

namespace EPHist {

using AxisVariant = std::variant<RegularAxis, VariableBinAxis>;

namespace Internal {
// Explicit specializations are only allowed at namespace scope.
template <typename Axis> struct AxisVariantIndex {};
template <> struct AxisVariantIndex<RegularAxis> {
  static constexpr std::size_t value = 0;
};
template <> struct AxisVariantIndex<VariableBinAxis> {
  static constexpr std::size_t value = 1;
};
} // namespace Internal

template <typename T> class EPHist final {
  std::unique_ptr<T[]> fData;
  std::size_t fNumBins;

  std::vector<AxisVariant> fAxes;

  class AxisPtr {
    std::uintptr_t fValue;
    static constexpr int Mask = 0x7;

  public:
    AxisPtr(AxisVariant &axis) {
      if (auto *regular = std::get_if<RegularAxis>(&axis)) {
        fValue = reinterpret_cast<std::uintptr_t>(regular);
      } else if (auto *variable = std::get_if<VariableBinAxis>(&axis)) {
        fValue = reinterpret_cast<std::uintptr_t>(variable);
      } else {
        assert(0 && "unknown axis type");
      }
      assert((fValue & Mask) == 0 && "lower bits of pointer not 0");
      std::size_t idx = axis.index();
      assert(idx <= Mask);
      fValue |= idx;
    }

    const void *GetPointer() const {
      return reinterpret_cast<const void *>(fValue & ~Mask);
    }
    int GetIndex() const { return fValue & Mask; }
  };
  std::vector<AxisPtr> fAxesPtrs;

  void SetupAxesPtrs() {
    fAxesPtrs.clear();
    for (auto &axis : fAxes) {
      fAxesPtrs.emplace_back(axis);
    }
  }

public:
  explicit EPHist(const std::vector<AxisVariant> &axes) : fAxes(axes) {
    fNumBins = 1;
    for (auto &&axis : axes) {
      if (auto *regular = std::get_if<RegularAxis>(&axis)) {
        fNumBins *= regular->GetNumBins();
      } else if (auto *variable = std::get_if<VariableBinAxis>(&axis)) {
        fNumBins *= variable->GetNumBins();
      }
    }
    fData.reset(new T[fNumBins]{});
    SetupAxesPtrs();
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
    for (std::size_t i = 0; i < fNumBins; i++) {
      fData[i] += other.fData[i];
    }
  }

  void Clear() {
    for (std::size_t i = 0; i < fNumBins; i++) {
      fData[i] = {};
    }
  }

  EPHist<T> Clone() {
    EPHist<T> h(fAxes);
    for (std::size_t i = 0; i < fNumBins; i++) {
      h.fData[i] += fData[i];
    }
    return h;
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
    const auto &axisPtr = fAxesPtrs[I];
    switch (axisPtr.GetIndex()) {
    case Internal::AxisVariantIndex<RegularAxis>::value: {
      const auto *regular =
          static_cast<const RegularAxis *>(axisPtr.GetPointer());
      bin *= regular->GetNumBins();
      bin += regular->ComputeBin(std::get<I>(args));
      break;
    }
    case Internal::AxisVariantIndex<VariableBinAxis>::value: {
      const auto *variable =
          static_cast<const VariableBinAxis *>(axisPtr.GetPointer());
      bin *= variable->GetNumBins();
      bin += variable->ComputeBin(std::get<I>(args));
      break;
    }
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
    const auto &axisPtr = fAxesPtrs[I];
    if (Internal::AxisVariantIndex<Axis>::value != axisPtr.GetIndex()) {
      throw std::invalid_argument("invalid axis type");
    }
    const auto &axis = *static_cast<const Axis *>(axisPtr.GetPointer());
    bin = bin * axis.GetNumBins() + axis.ComputeBin(arg);
    if constexpr (sizeof...(Axes) > 0) {
      return ComputeBin<I + 1, Axes...>(bin, args...);
    }
    return bin;
  }

public:
  template <typename... A> void Fill(const std::tuple<A...> &args) {
    if (sizeof...(A) != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    std::size_t bin = ComputeBin<0>(0, args);
    fData[bin]++;
  }

  template <typename... A> void Fill(const A &...args) {
    if (sizeof...(A) != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args) {
    if (sizeof...(Axes) != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    std::size_t bin = ComputeBin<0, Axes...>(0, args...);
    fData[bin]++;
  }
};

} // namespace EPHist

#endif
