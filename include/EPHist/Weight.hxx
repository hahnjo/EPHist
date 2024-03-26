// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_WEIGHT
#define EPHIST_WEIGHT

namespace EPHist {

struct Weight {
  double fValue;

  explicit Weight(double value) : fValue(value) {}
};

} // namespace EPHist

#endif
