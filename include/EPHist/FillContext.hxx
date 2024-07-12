// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_FILLCONTEXT
#define EPHIST_FILLCONTEXT

#include "EPHist.hxx"
#include "ParallelFillStrategy.hxx"

#include <cassert>
#include <memory>
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
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(std::forward_as_tuple(args...));
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

  static constexpr bool SupportsWeightedFill = EPHist<T>::SupportsWeightedFill;

  template <typename... A> void Fill(Weight w, const std::tuple<A...> &args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->FillAtomic(w, args);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->Fill(w, args);
      break;
    }
  }

  template <typename... A> void Fill(Weight w, const A &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(w, std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(Weight w, const typename Axes::ArgumentType &...args) {
    static_assert(
        SupportsWeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(Axes) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    switch (fStrategy) {
    case ParallelFillStrategy::Automatic:
    case ParallelFillStrategy::Atomic:
      fHist->template FillAtomic<Axes...>(w, args...);
      break;
    case ParallelFillStrategy::PerFillContext:
      assert(fLocalHist);
      fLocalHist->template Fill<Axes...>(w, args...);
      break;
    }
  }
};

} // namespace EPHist

#endif
