// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_BININDEXRANGE
#define EPHIST_BININDEXRANGE

#include "BinIndex.hxx"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>

namespace EPHist {

class BinIndexRange final {
  BinIndex fBegin;
  BinIndex fEnd;
  std::size_t fNumBins = 0;

public:
  BinIndexRange() = default;

  BinIndexRange(BinIndex begin, BinIndex end) : fBegin(begin), fEnd(end) {
    if (!begin.IsNormal()) {
      throw std::invalid_argument("begin must be a normal bin");
    }
    if (!end.IsNormal()) {
      throw std::invalid_argument("end must be a normal bin");
    }
    if (!(begin <= end)) {
      throw std::invalid_argument("begin must be smaller or equal end");
    }
  }

  BinIndex GetBegin() const { return fBegin; }
  BinIndex GetEnd() const { return fEnd; }

  class Iterator final {
    BinIndex fIndex;
    std::size_t fNumBins = 0;

  public:
    using difference_type = std::size_t;
    using value_type = BinIndex;
    using pointer = BinIndex *;
    using reference = BinIndex &;
    using iterator_category = std::forward_iterator_tag;

    Iterator() = default;
    Iterator(BinIndex index, std::size_t numBins)
        : fIndex(index), fNumBins(numBins) {}

    Iterator &operator++() {
      if (fIndex.IsUnderflow()) {
        fIndex = 0;
      } else if (fIndex.IsOverflow()) {
        fIndex = BinIndex();
      } else if (fIndex.IsInvalid()) {
        // This should never happen! In the worst case, when built with NDEBUG,
        // the iterator stays at Invalid.
        assert(0);
      } else {
        fIndex++;
        if (fIndex.GetIndex() == fNumBins) {
          fIndex = BinIndex::Overflow();
        }
      }
      return *this;
    }
    Iterator operator++(int) {
      Iterator old = *this;
      operator++();
      return old;
    }

    BinIndex operator*() const { return fIndex; }

    friend bool operator==(Iterator lhs, Iterator rhs) {
      return lhs.fIndex == rhs.fIndex && lhs.fNumBins == rhs.fNumBins;
    }
    friend bool operator!=(Iterator lhs, Iterator rhs) { return !(lhs == rhs); }
  };

  Iterator begin() const { return Iterator(fBegin, fNumBins); }
  Iterator end() const { return Iterator(fEnd, fNumBins); }

  bool IsFull() const { return fEnd.IsInvalid(); }
  BinIndexRange GetNormalRange(std::size_t numBins = 0) const {
    if (IsFull()) {
      // If this is the full range, return a new range for all normal bins.
      if (numBins == 0) {
        numBins = fNumBins;
      }
      assert(numBins > 0);
      return BinIndexRange(0, numBins);
    }
    // Otherwise just return a copy of this range.
    return *this;
  }

  friend bool operator==(const BinIndexRange &lhs, const BinIndexRange &rhs) {
    return lhs.fBegin == rhs.fBegin && lhs.fEnd == rhs.fEnd &&
           lhs.fNumBins == rhs.fNumBins;
  }

  friend bool operator!=(const BinIndexRange &lhs, const BinIndexRange &rhs) {
    return !(lhs == rhs);
  }

  static BinIndexRange Full(std::size_t numBins) {
    BinIndexRange full;
    full.fBegin = BinIndex::Underflow();
    full.fEnd = BinIndex();
    full.fNumBins = numBins;
    return full;
  }

  static BinIndexRange FullCategorical(std::size_t numBins) {
    BinIndexRange full;
    full.fBegin = 0;
    full.fEnd = BinIndex();
    full.fNumBins = numBins;
    return full;
  }
};

} // namespace EPHist

#endif
