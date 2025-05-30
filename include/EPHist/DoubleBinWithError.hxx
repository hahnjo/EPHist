// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_DOUBLEBINWITHERROR
#define EPHIST_DOUBLEBINWITHERROR

#include "Atomic.hxx"

namespace EPHist {

struct DoubleBinWithError {
  double fSum = 0;
  double fSum2 = 0;

  DoubleBinWithError &operator++() {
    fSum++;
    fSum2++;
    return *this;
  }

  DoubleBinWithError operator++(int) {
    DoubleBinWithError old = *this;
    operator++();
    return old;
  }

  DoubleBinWithError &operator+=(const DoubleBinWithError &rhs) {
    fSum += rhs.fSum;
    fSum2 += rhs.fSum2;
    return *this;
  }

  DoubleBinWithError &operator+=(double w) {
    fSum += w;
    fSum2 += w * w;
    return *this;
  }

  void AtomicInc() {
    Internal::AtomicInc(&fSum);
    Internal::AtomicInc(&fSum2);
  }

  void AtomicAdd(const DoubleBinWithError &rhs) {
    Internal::AtomicAdd(&fSum, rhs.fSum);
    Internal::AtomicAdd(&fSum2, rhs.fSum2);
  }

  void AtomicAddDouble(double w) {
    Internal::AtomicAdd(&fSum, w);
    Internal::AtomicAdd(&fSum2, w * w);
  }
};

} // namespace EPHist

#endif
