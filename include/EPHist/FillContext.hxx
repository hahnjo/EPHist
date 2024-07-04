// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_FILLCONTEXT
#define EPHIST_FILLCONTEXT

#include "EPHist.hxx"
#include "ParallelFillStrategy.hxx"

#include <cassert>
#include <tuple>

namespace EPHist {

template <typename T> class ParallelHelper;

template <typename T> class FillContext final {
  friend class ParallelHelper<T>;

private:
  EPHist<T> *fHist;
  ParallelFillStrategy fStrategy;

  explicit FillContext(EPHist<T> &hist, ParallelFillStrategy strategy)
      : fHist(&hist), fStrategy(strategy) {
    assert(fStrategy != ParallelFillStrategy::Automatic);
  }
  FillContext(const FillContext<T> &) = delete;
  FillContext(FillContext<T> &&) = default;
  FillContext<T> &operator=(const FillContext<T> &) = delete;
  FillContext<T> &operator=(FillContext<T> &&) = default;

public:
  ~FillContext() { Flush(); }

  void Flush() {
    // Nothing to do for now...
  }

  template <typename... A> void Fill(const std::tuple<A...> &args) {
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    fHist->FillAtomic(args);
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
    fHist->template FillAtomic<Axes...>(args...);
  }

  static constexpr bool WeightedFill = EPHist<T>::WeightedFill;

  template <typename... A> void Fill(Weight w, const std::tuple<A...> &args) {
    static_assert(
        WeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    fHist->FillAtomic(w, args);
  }

  template <typename... A> void Fill(Weight w, const A &...args) {
    static_assert(
        WeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(A) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    Fill(w, std::forward_as_tuple(args...));
  }

  template <class... Axes>
  void Fill(Weight w, const typename Axes::ArgumentType &...args) {
    static_assert(
        WeightedFill,
        "Fill with Weight is only supported for floating point bin types");
    if (sizeof...(Axes) != fHist->GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    fHist->template FillAtomic<Axes...>(w, args...);
  }
};

} // namespace EPHist

#endif
