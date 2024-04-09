// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_UTIL_CONVERTTOROOT
#define EPHIST_UTIL_CONVERTTOROOT

#include "../EPHist.hxx"

#include <TH1.h>

#include <memory>

namespace EPHist {
namespace Util {

std::unique_ptr<TH1I> ConvertToTH1I(const EPHist<int> &h);
std::unique_ptr<TH1F> ConvertToTH1F(const EPHist<float> &h);
std::unique_ptr<TH1D> ConvertToTH1D(const EPHist<double> &h);

} // namespace Util
} // namespace EPHist

#endif
