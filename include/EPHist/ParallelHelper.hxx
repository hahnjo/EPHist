// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_PARALLELHELPER
#define EPHIST_PARALLELHELPER

#include "EPHist.hxx"
#include "FillContext.hxx"
#include "ParallelFillStrategy.hxx"

#include <memory>
#include <mutex>
#include <vector>

namespace EPHist {

template <typename T> class ParallelHelper final {
private:
  std::shared_ptr<EPHist<T>> fHist;
  ParallelFillStrategy fStrategy;

  std::mutex fMutex;
  std::vector<std::weak_ptr<FillContext<T>>> fFillContexts;

public:
  explicit ParallelHelper(
      std::shared_ptr<EPHist<T>> hist,
      ParallelFillStrategy strategy = ParallelFillStrategy::Automatic)
      : fHist(std::move(hist)), fStrategy(strategy) {
    if (fStrategy == ParallelFillStrategy::Automatic) {
      // Default to atomic filling for the moment...
      fStrategy = ParallelFillStrategy::Atomic;
    }
  }
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
    std::shared_ptr<FillContext<T>> context(
        new FillContext<T>(*fHist, fStrategy));
    fFillContexts.push_back(context);
    return context;
  }
};

} // namespace EPHist

#endif
