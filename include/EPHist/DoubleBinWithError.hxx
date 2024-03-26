// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_DOUBLEBINWITHERROR
#define EPHIST_DOUBLEBINWITHERROR

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

  DoubleBinWithError operator+=(double value) {
    fSum += value;
    fSum2 += value * value;
    return *this;
  }
};

} // namespace EPHist

#endif
