// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_UTIL_RDATAFRAMEHELPER
#define EPHIST_UTIL_RDATAFRAMEHELPER

#include "../EPHist.hxx"

#include <ROOT/RDF/RActionImpl.hxx>

#include <memory>

class TTreeReader;

namespace EPHist {
namespace Util {

template <typename T>
class EPHistFillAddHelper
    : public ROOT::Detail::RDF::RActionImpl<EPHistFillAddHelper<T>> {
public:
  using Result_t = EPHist<T>;

private:
  std::vector<std::shared_ptr<Result_t>> fHists;

public:
  template <typename... Args>
  EPHistFillAddHelper(unsigned int nSlots, const Args &...args) {
    for (unsigned int i = 0; i < nSlots; i++) {
      fHists.emplace_back(std::make_shared<Result_t>(args...));
    }
  }
  EPHistFillAddHelper(const EPHistFillAddHelper &) = delete;
  EPHistFillAddHelper(EPHistFillAddHelper &&) = default;
  std::shared_ptr<Result_t> GetResultPtr() const { return fHists[0]; }
  void Initialize() {}
  void InitTask(TTreeReader *, unsigned int) {}
  template <typename... ColumnTypes>
  void Exec(unsigned int slot, ColumnTypes... values) {
    fHists[slot]->Fill(values...);
  }
  void Finalize() {
    auto &res = fHists[0];
    for (std::size_t i = 1; i < fHists.size(); i++) {
      res->Add(*fHists[i]);
    }
  }

  std::string GetActionName() const { return "EPHistFillAddHelper"; }
};

template <typename T>
class EPHistFillAtomicHelper
    : public ROOT::Detail::RDF::RActionImpl<EPHistFillAtomicHelper<T>> {
public:
  using Result_t = EPHist<T>;

private:
  std::shared_ptr<Result_t> fHist;

public:
  template <typename... Args> EPHistFillAtomicHelper(const Args &...args) {
    fHist = std::make_shared<Result_t>(args...);
  }
  EPHistFillAtomicHelper(EPHistFillAtomicHelper &&) = default;
  EPHistFillAtomicHelper(const EPHistFillAtomicHelper &) = delete;
  std::shared_ptr<Result_t> GetResultPtr() const { return fHist; }
  void Initialize() {}
  void InitTask(TTreeReader *, unsigned int) {}
  template <typename... ColumnTypes>
  void Exec(unsigned int /*slot*/, ColumnTypes... values) {
    fHist->FillAtomic(values...);
  }
  void Finalize() {}

  std::string GetActionName() const { return "EPHistFillAtomicHelper"; }
};

} // namespace Util
} // namespace EPHist

#endif
