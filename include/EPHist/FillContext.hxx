// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_FILLCONTEXT
#define EPHIST_FILLCONTEXT

#include "EPHist.hxx"
#include "ParallelFillStrategy.hxx"
#include "TypeTraits.hxx"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <tuple>

namespace EPHist {

template <typename T> class ParallelHelper;

template <typename T> class FillContext final {
  friend class ParallelHelper<T>;

private:
  EPHist<T> *fHist;
  ParallelFillStrategy fStrategy;

  std::unique_ptr<EPHist<T>> fLocalHist;

  explicit FillContext(EPHist<T> &hist, ParallelFillStrategy strategy)
      : fHist(&hist), fStrategy(strategy) {
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
      assert(0);
      // Graceful switch to atomic strategy, in case assertions are disabled.
      fStrategy = ParallelFillStrategy::Atomic;
    case ParallelFillStrategy::Atomic:
      // Nothing to do...
      break;
    case ParallelFillStrategy::PerFillContext:
      fLocalHist.reset(new EPHist<T>(fHist->GetAxes()));
      break;
    }
  }
  FillContext(const FillContext<T> &) = delete;
  FillContext(FillContext<T> &&) = default;
  FillContext<T> &operator=(const FillContext<T> &) = delete;
  FillContext<T> &operator=(FillContext<T> &&) = default;

public:
  ~FillContext() { Flush(); }

  void Flush() {
    if (fStrategy == ParallelFillStrategy::PerFillContext) {
      assert(fLocalHist);
      fHist->AddAtomic(*fLocalHist);
    }
  }

  static constexpr bool SupportsWeightedFill = EPHist<T>::SupportsWeightedFill;

private:
  template <std::size_t N, typename... A>
  void FillImpl(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    assert(N == fHist->GetNumDimensions());
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->template FillAtomicImpl<N>(args, w);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->template FillImpl<N>(args, w);
      break;
    }
  }

public:
  template <typename... A> void Fill(const std::tuple<A...> &args) {
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->FillAtomic(args);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->Fill(args);
      break;
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
      if (sizeof...(A) - 1 != fHist->GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      FillImpl<sizeof...(A) - 1>(t, std::get<sizeof...(A) - 1>(t));
    } else {
      if (sizeof...(A) != fHist->GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      Fill(t);
    }
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args) {
    if (sizeof...(Axes) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->template FillAtomic<Axes...>(args...);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->template Fill<Axes...>(args...);
      break;
    }
  }

  template <typename... A> void Fill(const std::tuple<A...> &args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A)>(args, w);
  }

  template <class... Axes>
  void Fill(const typename Axes::ArgumentType &...args, Weight w) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(Axes) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->template FillAtomic<Axes...>(args..., w);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->template Fill<Axes...>(args..., w);
      break;
    }
  }
};

} // namespace EPHist

#endif
