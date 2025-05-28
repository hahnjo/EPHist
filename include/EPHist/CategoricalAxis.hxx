// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_CATEGORICALAXIS
#define EPHIST_CATEGORICALAXIS

#include "BinIndex.hxx"
#include "BinIndexRange.hxx"

#include <cassert>
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace EPHist {

class CategoricalAxis final {
public:
  using ArgumentType = std::string_view;

private:
  std::vector<std::string> fCategories;
  bool fEnableOverflowBin;

public:
  explicit CategoricalAxis(std::vector<std::string> categories,
                           bool enableOverflowBin = true)
      : fCategories(std::move(categories)),
        fEnableOverflowBin(enableOverflowBin) {
    auto set = std::unordered_set(fCategories.begin(), fCategories.end());
    if (set.size() != fCategories.size()) {
      throw std::invalid_argument("duplicate categories");
    }
  }

  std::size_t GetNumBins() const { return fCategories.size(); }
  std::size_t GetTotalNumBins() const {
    return fEnableOverflowBin ? fCategories.size() + 1 : fCategories.size();
  }
  const std::vector<std::string> &GetCategories() const { return fCategories; }
  std::string GetCategory(std::size_t bin) const { return fCategories[bin]; }
  bool IsOverflowBinEnabled() const { return fEnableOverflowBin; }

  std::pair<std::size_t, bool> GetBin(BinIndex index) const {
    if (index.IsUnderflow()) {
      return {0, false};
    } else if (index.IsOverflow()) {
      return {fCategories.size(), fEnableOverflowBin};
    } else if (index.IsInvalid()) {
      return {0, false};
    }
    assert(index.IsNormal());
    std::size_t bin = index.GetIndex();
    return {bin, bin < fCategories.size()};
  }

  std::pair<std::size_t, bool> ComputeBin(std::string_view x) const {
    // FIXME: Optimize with hashing... (?)
    for (std::size_t bin = 0; bin < fCategories.size(); bin++) {
      if (fCategories[bin] == x) {
        return {bin, true};
      }
    }

    // Category not found
    return {fCategories.size(), fEnableOverflowBin};
  }

  CategoricalAxis Slice(const BinIndexRange &range) const {
    const auto normalRange = range.GetNormalRange(fCategories.size());

    const auto begin = normalRange.GetBegin();
    const auto end = normalRange.GetEnd();
    assert(begin.IsNormal());
    assert(end.IsNormal());
    assert(begin <= end);

    std::vector categories(fCategories.begin() + begin.GetIndex(),
                           fCategories.begin() + end.GetIndex());
    // Always enable overflow bin.
    const auto enableOverflowBin = true;
    return CategoricalAxis(std::move(categories), enableOverflowBin);
  }

  friend bool operator==(const CategoricalAxis &lhs,
                         const CategoricalAxis &rhs) {
    return lhs.fCategories == rhs.fCategories &&
           lhs.fEnableOverflowBin == rhs.fEnableOverflowBin;
  }
};

} // namespace EPHist

#endif
