// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_AXES
#define EPHIST_AXES

#include "RegularAxis.hxx"
#include "VariableBinAxis.hxx"

#include <cstddef>
#include <cstdint>
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

namespace Detail {

class Axes {
  std::vector<AxisVariant> fAxes;

  class AxisPtr {
    std::uintptr_t fValue;
    static constexpr int Mask = 0x7;

  public:
    AxisPtr(const AxisVariant &axis) {
      if (const auto *regular = std::get_if<RegularAxis>(&axis)) {
        fValue = reinterpret_cast<std::uintptr_t>(regular);
      } else if (const auto *variable = std::get_if<VariableBinAxis>(&axis)) {
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

  static std::vector<AxisPtr>
  PrepareAxesPtrs(const std::vector<AxisVariant> &axes) {
    std::vector<AxisPtr> axesPtrs;
    for (const auto &axis : axes) {
      axesPtrs.emplace_back(axis);
    }
    return axesPtrs;
  }

public:
  explicit Axes(const std::vector<AxisVariant> &axes)
      : fAxes(axes), fAxesPtrs(PrepareAxesPtrs(fAxes)) {}

  std::size_t GetNumDimensions() const { return fAxes.size(); }
  const std::vector<AxisVariant> &GetVector() const { return fAxes; }

  std::size_t ComputeTotalNumBins() const {
    std::size_t totalNumBins = 1;
    for (auto &&axis : fAxes) {
      if (auto *regular = std::get_if<RegularAxis>(&axis)) {
        totalNumBins *= regular->GetTotalNumBins();
      } else if (auto *variable = std::get_if<VariableBinAxis>(&axis)) {
        totalNumBins *= variable->GetTotalNumBins();
      }
    }
    return totalNumBins;
  }

private:
  template <std::size_t I, typename... A>
  std::pair<std::size_t, bool> ComputeBin(std::size_t bin,
                                          const std::tuple<A...> &args) const {
    const auto &axisPtr = fAxesPtrs[I];
    std::pair<std::size_t, bool> axisBin;
    switch (axisPtr.GetIndex()) {
    case Internal::AxisVariantIndex<RegularAxis>::value: {
      const auto *regular =
          static_cast<const RegularAxis *>(axisPtr.GetPointer());
      bin *= regular->GetTotalNumBins();
      axisBin = regular->ComputeBin(std::get<I>(args));
      break;
    }
    case Internal::AxisVariantIndex<VariableBinAxis>::value: {
      const auto *variable =
          static_cast<const VariableBinAxis *>(axisPtr.GetPointer());
      bin *= variable->GetTotalNumBins();
      axisBin = variable->ComputeBin(std::get<I>(args));
      break;
    }
    }
    if (!axisBin.second) {
      return {0, false};
    }
    bin += axisBin.first;
    if constexpr (I + 1 < sizeof...(A)) {
      return ComputeBin<I + 1>(bin, args);
    }
    return {bin, true};
  }

  template <std::size_t I, class Axis, class... Axes>
  std::pair<std::size_t, bool>
  ComputeBin(std::size_t bin, const typename Axis::ArgumentType &arg,
             const typename Axes::ArgumentType &...args) const {
    const auto &axisPtr = fAxesPtrs[I];
    if (Internal::AxisVariantIndex<Axis>::value != axisPtr.GetIndex()) {
      throw std::invalid_argument("invalid axis type");
    }
    const auto &axis = *static_cast<const Axis *>(axisPtr.GetPointer());
    bin = bin * axis.GetTotalNumBins();
    auto axisBin = axis.ComputeBin(arg);
    if (!axisBin.second) {
      return {0, false};
    }
    bin += axisBin.first;
    if constexpr (sizeof...(Axes) > 0) {
      return ComputeBin<I + 1, Axes...>(bin, args...);
    }
    return {bin, true};
  }

public:
  template <typename... A>
  std::pair<std::size_t, bool> ComputeBin(const std::tuple<A...> &args) const {
    return ComputeBin<0>(0, args);
  }

  template <class... Axes>
  std::pair<std::size_t, bool>
  ComputeBin(const typename Axes::ArgumentType &...args) const {
    return ComputeBin<0, Axes...>(0, args...);
  }

  friend bool operator==(const Axes &lhs, const Axes &rhs) {
    return lhs.fAxes == rhs.fAxes;
  }
  friend bool operator!=(const Axes &lhs, const Axes &rhs) {
    return !operator==(lhs, rhs);
  }
};

} // namespace Detail

} // namespace EPHist

#endif
