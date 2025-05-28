// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_EPHIST
#define EPHIST_EPHIST

#include "Atomic.hxx"
#include "Axes.hxx"
#include "BinIndex.hxx"
#include "DoubleBinWithError.hxx"
#include "TypeTraits.hxx"
#include "Weight.hxx"

#include <array>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace EPHist {

template <typename T> class FillContext;

template <typename T> class EPHist final {
  friend class FillContext<T>;

public:
  using BinContentType = T;

private:
  std::vector<T> fData;

  Detail::Axes fAxes;

public:
  explicit EPHist(std::vector<AxisVariant> axes) : fAxes(std::move(axes)) {
    fData.resize(fAxes.ComputeTotalNumBins());
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
    for (std::size_t i = 0; i < fData.size(); i++) {
      fData[i] += other.fData[i];
    }
  }

  void AddAtomic(const EPHist<T> &other) {
    if (fAxes != other.fAxes) {
      throw std::invalid_argument("axes configuration not identical");
    }
    for (std::size_t i = 0; i < fData.size(); i++) {
      Internal::AtomicAdd(&fData[i], other.fData[i]);
    }
  }

  void Clear() {
    for (std::size_t i = 0; i < fData.size(); i++) {
      fData[i] = {};
    }
  }

  EPHist<T> Clone() {
    EPHist<T> h(fAxes.GetVector());
    for (std::size_t i = 0; i < fData.size(); i++) {
      h.fData[i] += fData[i];
    }
    return h;
  }

  const T &GetBinContent(std::size_t bin) const {
    assert(bin >= 0 && bin < fData.size());
    return fData[bin];
  }
  template <std::size_t N>
  const T &GetBinContentAt(const std::array<BinIndex, N> &args) const {
    if (N != fAxes.GetNumDimensions()) {
      throw std::invalid_argument(
          "invalid number of arguments to GetBinContent");
    }
    auto bin = fAxes.ComputeBin(args);
    if (!bin.second) {
      throw std::invalid_argument("bin not found");
    }
    return fData[bin.first];
  }
  template <typename... A> const T &GetBinContentAt(const A &...args) const {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument(
          "invalid number of arguments to GetBinContent");
    }
    std::array<BinIndex, sizeof...(A)> a{args...};
    return GetBinContentAt(a);
  }
  std::size_t GetTotalNumBins() const { return fData.size(); }

  const std::vector<AxisVariant> &GetAxes() const { return fAxes.GetVector(); }
  std::size_t GetNumDimensions() const { return fAxes.GetNumDimensions(); }

  static constexpr bool SupportsWeightedFill =
      std::is_floating_point_v<T> || std::is_same_v<T, DoubleBinWithError>;

private:
  template <std::size_t N, typename... A>
  void FillImpl(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    assert(N == fAxes.GetNumDimensions());
    auto bin = fAxes.ComputeBin<N>(args);
    if (bin.second) {
      fData[bin.first] += w.fValue;
    }
  }

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
    auto t = std::forward_as_tuple(args...);
    // Could use std::tuple_element_t<sizeof...(A) - 1, decltype(t)>, but that
    // would be const Weight &
    if constexpr (std::is_same_v<typename Internal::LastType<A...>::type,
                                 Weight>) {
      static_assert(
          SupportsWeightedFill,
          "Fill with Weight is only supported for floating point bin types");
      if (sizeof...(A) - 1 != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      FillImpl<sizeof...(A) - 1>(t, std::get<sizeof...(A) - 1>(t));
    } else {
      if (sizeof...(A) != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      Fill(t);
    }
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

  template <typename... A> void Fill(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A)>(args, w);
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args, Weight w) {
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

private:
  template <std::size_t N, typename... A>
  void FillAtomicImpl(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    assert(N == fAxes.GetNumDimensions());
    auto bin = fAxes.ComputeBin<N>(args);
    if (bin.second) {
      Internal::AtomicAddDouble(&fData[bin.first], w.fValue);
    }
  }

public:
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
    auto t = std::forward_as_tuple(args...);
    // Could use std::tuple_element_t<sizeof...(A) - 1, decltype(t)>, but that
    // would be const Weight &
    if constexpr (std::is_same_v<typename Internal::LastType<A...>::type,
                                 Weight>) {
      static_assert(
          SupportsWeightedFill,
          "Fill with Weight is only supported for floating point bin types");
      if (sizeof...(A) - 1 != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      FillAtomicImpl<sizeof...(A) - 1>(t, std::get<sizeof...(A) - 1>(t));
    } else {
      if (sizeof...(A) != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      FillAtomic(t);
    }
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
  void FillAtomic(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillAtomicImpl<sizeof...(A)>(args, w);
  }

  template <class... Axes>
  void FillAtomic(const typename Axes::ArgumentType &...args, Weight w) {
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

  template <std::size_t N>
  EPHist<T> Slice(const std::array<BinIndexRange, N> &ranges) const {
    if (N != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Slice");
    }

    // Create the sliced axes.
    std::vector<AxisVariant> slicedAxes = fAxes.Slice(ranges);

    // Collect full ranges of the original histogram and normalize the full
    // ranges potentially passed in by the user.
    std::array<BinIndexRange, N> fullRanges;
    std::array<BinIndexRange, N> normalRanges;
    for (std::size_t i = 0; i < N; i++) {
      const auto &axis = fAxes.GetVector()[i];
      switch (axis.index()) {
      case Internal::AxisVariantIndex<RegularAxis>::value: {
        const auto *regular = std::get_if<RegularAxis>(&axis);
        const std::size_t numBins = regular->GetNumBins();
        if (regular->AreUnderflowOverflowBinsEnabled()) {
          fullRanges[i] = BinIndexRange::Full(numBins);
        } else {
          fullRanges[i] = BinIndexRange(0, numBins);
        }
        normalRanges[i] = ranges[i].GetNormalRange(numBins);
        break;
      }
      case Internal::AxisVariantIndex<VariableBinAxis>::value: {
        const auto *variable = std::get_if<VariableBinAxis>(&axis);
        const std::size_t numBins = variable->GetNumBins();
        if (variable->AreUnderflowOverflowBinsEnabled()) {
          fullRanges[i] = BinIndexRange::Full(numBins);
        } else {
          fullRanges[i] = BinIndexRange(0, numBins);
        }
        normalRanges[i] = ranges[i].GetNormalRange(numBins);
        break;
      }
      }
    }

    auto getSliceIndex = [&](std::size_t i, BinIndex index) {
      if (index.IsNormal()) {
        // Compare the index to normalRanges[i] and map into the underflow or
        // overflow bin if outside.
        if (index < normalRanges[i].GetBegin()) {
          return BinIndex::Underflow();
        } else if (index >= normalRanges[i].GetEnd()) {
          return BinIndex::Overflow();
        }

        // Otherwise adjust the index by the begin index.
        const auto beginIndex = normalRanges[i].GetBegin().GetIndex();
        return BinIndex(index.GetIndex() - beginIndex);
      }

      // All other bins map to themselves, in particular the underflow and
      // overflow bins.
      return index;
    };

    // Create the sliced histogram and copy data.
    EPHist<T> slice(std::move(slicedAxes));
    std::array<BinIndexRange::Iterator, N> origIndexIterator;
    std::array<BinIndex, N> origIndexes;
    std::array<BinIndex, N> sliceIndexes;
    for (std::size_t i = 0; i < N; i++) {
      origIndexIterator[i] = fullRanges[i].begin();
      origIndexes[i] = *origIndexIterator[i];
      sliceIndexes[i] = getSliceIndex(i, origIndexes[i]);
    }

    while (true) {
      const auto origBin = fAxes.ComputeBin(origIndexes);
      assert(origBin.second);
      const auto sliceBin = slice.fAxes.ComputeBin(sliceIndexes);
      assert(sliceBin.second);
      slice.fData[sliceBin.first] += fData[origBin.first];

      // Advance the indices.
      bool shouldContinueAdvance = true;
      for (std::size_t j = 0; j < N; j++) {
        // Reverse iteration order to improve performance by advancing the
        // innermost index first.
        const std::size_t i = N - 1 - j;

        shouldContinueAdvance = false;
        // Advance this iterator.
        origIndexIterator[i]++;
        // If we reached the end, wrap around.
        if (origIndexIterator[i] == fullRanges[i].end()) {
          origIndexIterator[i] = fullRanges[i].begin();
          shouldContinueAdvance = true;
        }
        // Get the index by dereferencing the iterator.
        origIndexes[i] = *origIndexIterator[i];
        sliceIndexes[i] = getSliceIndex(i, origIndexes[i]);

        if (!shouldContinueAdvance) {
          break;
        }
      }
      if (shouldContinueAdvance) {
        // No more index found to advance, we are done.
        break;
      }
    }

    return slice;
  }

  template <typename... A> EPHist<T> Slice(const A &...args) const {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Slice");
    }
    std::array<BinIndexRange, sizeof...(A)> ranges{args...};
    return Slice(ranges);
  }
};

} // namespace EPHist

#endif
