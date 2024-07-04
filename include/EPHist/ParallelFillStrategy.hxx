// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_PARALLELFILLSTRATEGY
#define EPHIST_PARALLELFILLSTRATEGY

namespace EPHist {

enum class ParallelFillStrategy {
  Automatic = 0,
  Atomic = 1,
  PerFillContext = 2,
};

} // namespace EPHist

#endif
