// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_PARALLELHELPER
#define EPHIST_PARALLELHELPER

#include "EPHist.hxx"

#include <memory>
#include <mutex>
#include <vector>

namespace EPHist {

template <typename T> class ParallelHelper;

template <typename T> class FillContext final {
  friend class ParallelHelper<T>;

private:
  EPHist<T> *fHist;

  explicit FillContext(EPHist<T> &hist) : fHist(&hist) {}
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

template <typename T> class ParallelHelper final {
private:
  std::shared_ptr<EPHist<T>> fHist;

  std::mutex fMutex;
  std::vector<std::weak_ptr<FillContext<T>>> fFillContexts;

public:
  explicit ParallelHelper(std::shared_ptr<EPHist<T>> hist)
      : fHist(std::move(hist)) {}
  ParallelHelper(const ParallelHelper<T> &) = delete;
  ParallelHelper(ParallelHelper<T> &&) = delete;
  ParallelHelper<T> &operator=(const ParallelHelper<T> &) = delete;
  ParallelHelper<T> &operator=(ParallelHelper<T> &&) = delete;
  ~ParallelHelper() {
    for (const auto &context : fFillContexts) {
      assert(context.expired());
    }
    Flush();
  }

  void Flush() {
    // Nothing to do for now...
  }

  std::shared_ptr<FillContext<T>> CreateFillContext() {
    std::lock_guard g(fMutex);

    // Cannot use std::make_shared because the constructor of FillContext is
    // private. Also it would mean that the (direct) memory of all contexts
    // stays around until the vector of weak_ptr's is cleared.
    std::shared_ptr<FillContext<T>> context(new FillContext<T>(*fHist));
    fFillContexts.push_back(context);
    return context;
  }
};

} // namespace EPHist

#endif
