// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_AXES
#define EPHIST_AXES

#include "BinIndex.hxx"
#include "BinIndexRange.hxx"
#include "CategoricalAxis.hxx"
#include "RegularAxis.hxx"
#include "VariableBinAxis.hxx"

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace EPHist {

template <typename T> class EPHist;
template <bool WithError> class Profile;

using AxisVariant = std::variant<RegularAxis, VariableBinAxis, CategoricalAxis>;

namespace Internal {
// Explicit specializations are only allowed at namespace scope.
template <typename Axis> struct AxisVariantIndex {};
template <> struct AxisVariantIndex<RegularAxis> {
  static constexpr std::size_t value = 0;
};
template <> struct AxisVariantIndex<VariableBinAxis> {
  static constexpr std::size_t value = 1;
};
template <> struct AxisVariantIndex<CategoricalAxis> {
  static constexpr std::size_t value = 2;
};
} // namespace Internal

namespace Detail {

class Axes final {
  template <typename T> friend class ::EPHist::EPHist;
  template <bool WithError> friend class ::EPHist::Profile;

  std::vector<AxisVariant> fAxes;

public:
  explicit Axes(std::vector<AxisVariant> axes) : fAxes(std::move(axes)) {}

  std::size_t GetNumDimensions() const { return fAxes.size(); }
  const std::vector<AxisVariant> &GetVector() const { return fAxes; }

  std::size_t ComputeTotalNumBins() const {
    std::size_t totalNumBins = 1;
    for (auto &&axis : fAxes) {
      if (auto *regular = std::get_if<RegularAxis>(&axis)) {
        totalNumBins *= regular->GetTotalNumBins();
      } else if (auto *variable = std::get_if<VariableBinAxis>(&axis)) {
        totalNumBins *= variable->GetTotalNumBins();
      } else if (auto *categorical = std::get_if<CategoricalAxis>(&axis)) {
        totalNumBins *= categorical->GetTotalNumBins();
      }
    }
    return totalNumBins;
  }

private:
  template <std::size_t I, std::size_t N, typename... A>
  std::pair<std::size_t, bool> ComputeBin(std::size_t bin,
                                          const std::tuple<A...> &args) const {
    using ArgumentType = std::tuple_element_t<I, std::tuple<A...>>;
    const auto &axis = fAxes[I];
    std::pair<std::size_t, bool> axisBin;
    switch (axis.index()) {
    case Internal::AxisVariantIndex<RegularAxis>::value: {
      if constexpr (std::is_convertible_v<ArgumentType,
                                          RegularAxis::ArgumentType>) {
        const auto *regular = std::get_if<RegularAxis>(&axis);
        bin *= regular->GetTotalNumBins();
        axisBin = regular->ComputeBin(std::get<I>(args));
      } else {
        throw std::invalid_argument("cannot convert argument");
      }
      break;
    }
    case Internal::AxisVariantIndex<VariableBinAxis>::value: {
      if constexpr (std::is_convertible_v<ArgumentType,
                                          VariableBinAxis::ArgumentType>) {
        const auto *variable = std::get_if<VariableBinAxis>(&axis);
        bin *= variable->GetTotalNumBins();
        axisBin = variable->ComputeBin(std::get<I>(args));
      } else {
        throw std::invalid_argument("cannot convert argument");
      }
      break;
    }
    case Internal::AxisVariantIndex<CategoricalAxis>::value: {
      if constexpr (std::is_convertible_v<ArgumentType,
                                          CategoricalAxis::ArgumentType>) {
        const auto *categorical = std::get_if<CategoricalAxis>(&axis);
        bin *= categorical->GetTotalNumBins();
        axisBin = categorical->ComputeBin(std::get<I>(args));
      } else {
        throw std::invalid_argument("cannot convert argument");
      }
      break;
    }
    }
    if (!axisBin.second) {
      return {0, false};
    }
    bin += axisBin.first;
    if constexpr (I + 1 < N) {
      return ComputeBin<I + 1, N>(bin, args);
    }
    return {bin, true};
  }

  // Private function to be used when the tuple has more elements than
  // this object has dimensions - additional elements are ignored.
  template <std::size_t N, typename... A>
  std::pair<std::size_t, bool> ComputeBin(const std::tuple<A...> &args) const {
    assert(N == fAxes.size());
    return ComputeBin<0, N>(0, args);
  }

  template <std::size_t I, class Axis, class... Axes>
  std::pair<std::size_t, bool>
  ComputeBin(std::size_t bin, const typename Axis::ArgumentType &arg,
             const typename Axes::ArgumentType &...args) const {
    const auto &axisVariant = fAxes[I];
    if (Internal::AxisVariantIndex<Axis>::value != axisVariant.index()) {
      throw std::invalid_argument("invalid axis type");
    }
    const auto &axis = *std::get_if<Axis>(&axisVariant);
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
    if (sizeof...(A) != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to ComputeBin");
    }
    return ComputeBin<0, sizeof...(A)>(0, args);
  }

  template <class... Axes>
  std::pair<std::size_t, bool>
  ComputeBin(const typename Axes::ArgumentType &...args) const {
    if (sizeof...(Axes) != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to ComputeBin");
    }
    return ComputeBin<0, Axes...>(0, args...);
  }

  template <std::size_t N>
  std::pair<std::size_t, bool>
  ComputeBin(const std::array<BinIndex, N> &args) const {
    if (N != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to ComputeBin");
    }
    std::size_t bin = 0;
    for (std::size_t i = 0; i < N; i++) {
      const auto &index = args[i];
      const auto &axis = fAxes[i];
      std::pair<std::size_t, bool> axisBin;
      switch (axis.index()) {
      case Internal::AxisVariantIndex<RegularAxis>::value: {
        const auto *regular = std::get_if<RegularAxis>(&axis);
        bin *= regular->GetTotalNumBins();
        axisBin = regular->GetBin(index);
        break;
      }
      case Internal::AxisVariantIndex<VariableBinAxis>::value: {
        const auto *variable = std::get_if<VariableBinAxis>(&axis);
        bin *= variable->GetTotalNumBins();
        axisBin = variable->GetBin(index);
        break;
      }
      case Internal::AxisVariantIndex<CategoricalAxis>::value: {
        const auto *categorical = std::get_if<CategoricalAxis>(&axis);
        bin *= categorical->GetTotalNumBins();
        axisBin = categorical->GetBin(index);
        break;
      }
      }
      if (!axisBin.second) {
        return {0, false};
      }
      bin += axisBin.first;
    }
    return {bin, true};
  }

  template <std::size_t N>
  std::vector<AxisVariant> Slice(const std::array<BinIndexRange, N> &ranges) const {
    if (N != fAxes.size()) {
      throw std::invalid_argument("invalid number of arguments to Slice");
    }
    std::vector<AxisVariant> axes;
    axes.reserve(N);
    for (std::size_t i = 0; i < N; i++) {
      const auto &range = ranges[i];
      const auto &axis = fAxes[i];
      switch (axis.index()) {
      case Internal::AxisVariantIndex<RegularAxis>::value: {
        const auto *regular = std::get_if<RegularAxis>(&axis);
        axes.push_back(regular->Slice(range));
        break;
      }
      case Internal::AxisVariantIndex<VariableBinAxis>::value: {
        const auto *variable = std::get_if<VariableBinAxis>(&axis);
        axes.push_back(variable->Slice(range));
        break;
      }
      }
    }
    return axes;
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
