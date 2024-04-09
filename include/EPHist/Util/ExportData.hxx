// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_UTIL_EXPORTDATA
#define EPHIST_UTIL_EXPORTDATA

#include "../EPHist.hxx"

#include <ostream>

namespace EPHist {
namespace Util {

class EPHistForExport {
public:
  virtual const std::vector<AxisVariant> &GetAxes() const = 0;
  virtual std::size_t GetNumDimensions() const = 0;
  virtual void PrintBinContent(std::size_t bin, std::ostream &os) const = 0;
};

template <typename T> class EPHistForExportT final : public EPHistForExport {
  const EPHist<T> *fHist;

public:
  EPHistForExportT(const EPHist<T> &h) : fHist(&h) {}

  const std::vector<AxisVariant> &GetAxes() const override {
    return fHist->GetAxes();
  }
  std::size_t GetNumDimensions() const override {
    return fHist->GetNumDimensions();
  }
  void PrintBinContent(std::size_t bin, std::ostream &os) const override {
    os << fHist->GetBinContent(bin);
  }
};

// Export data in a textual format, that can for example be used with gnuplot,
// Matplotlib, and PGFPlots.
void ExportTextData(const EPHistForExport &h, std::ostream &os);
template <typename T>
void ExportTextData(const EPHist<T> &h, std::ostream &os) {
  ExportTextData(EPHistForExportT<T>(h), os);
}

} // namespace Util
} // namespace EPHist

#endif
