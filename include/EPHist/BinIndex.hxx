// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_BININDEX
#define EPHIST_BININDEX

#include <cassert>
#include <cstddef>

namespace EPHist {

class BinIndex final {
  static constexpr std::size_t UnderflowIndex = -3;
  static constexpr std::size_t OverflowIndex = -2;
  static constexpr std::size_t InvalidIndex = -1;

  std::size_t fIndex = InvalidIndex;

public:
  BinIndex() = default;

  BinIndex(std::size_t index) : fIndex(index) { assert(IsNormal()); }

  BinIndex &operator+=(std::size_t a) {
    assert(IsNormal());
    fIndex += a;
    // TODO: is it possible to catch overflows?
    assert(IsNormal());
    return *this;
  }

  BinIndex operator+(std::size_t a) const {
    BinIndex ret = *this;
    ret += a;
    return ret;
  }

  BinIndex &operator++() {
    operator+=(1);
    return *this;
  }

  BinIndex operator++(int) {
    BinIndex old = *this;
    operator++();
    return old;
  }

  BinIndex &operator-=(std::size_t a) {
    assert(IsNormal());
    assert(fIndex >= a);
    fIndex -= a;
    assert(IsNormal());
    return *this;
  }

  BinIndex operator-(std::size_t a) const {
    BinIndex ret = *this;
    ret -= a;
    return ret;
  }

  BinIndex &operator--() {
    operator-=(1);
    return *this;
  }

  BinIndex operator--(int) {
    BinIndex old = *this;
    operator--();
    return old;
  }

  friend bool operator==(BinIndex lhs, BinIndex rhs) {
    if (lhs.IsNormal() && rhs.IsNormal()) {
      return lhs.fIndex == rhs.fIndex;
    } else if (lhs.IsUnderflow() && rhs.IsUnderflow()) {
      return true;
    } else if (lhs.IsOverflow() && rhs.IsOverflow()) {
      return true;
    } else if (lhs.IsInvalid() && rhs.IsInvalid()) {
      return true;
    }
    return false;
  }

  friend bool operator!=(BinIndex lhs, BinIndex rhs) { return !(lhs == rhs); }

  friend bool operator<(BinIndex lhs, BinIndex rhs) {
    if (lhs.IsNormal() && rhs.IsNormal()) {
      return lhs.fIndex < rhs.fIndex;
    }
    return false;
  }

  friend bool operator<=(BinIndex lhs, BinIndex rhs) {
    if (lhs == rhs) {
      return true;
    }
    if (lhs.IsNormal() && rhs.IsNormal()) {
      return lhs.fIndex <= rhs.fIndex;
    }
    return false;
  }

  friend bool operator>(BinIndex lhs, BinIndex rhs) {
    if (lhs.IsNormal() && rhs.IsNormal()) {
      return lhs.fIndex > rhs.fIndex;
    }
    return false;
  }

  friend bool operator>=(BinIndex lhs, BinIndex rhs) {
    if (lhs == rhs) {
      return true;
    }
    if (lhs.IsNormal() && rhs.IsNormal()) {
      return lhs.fIndex >= rhs.fIndex;
    }
    return false;
  }

  std::size_t GetIndex() const {
    assert(IsNormal());
    return fIndex;
  }
  bool IsNormal() const { return fIndex < UnderflowIndex; }
  bool IsUnderflow() const { return fIndex == UnderflowIndex; }
  bool IsOverflow() const { return fIndex == OverflowIndex; }
  bool IsInvalid() const { return fIndex == InvalidIndex; }

  static BinIndex Underflow() {
    BinIndex underflow;
    underflow.fIndex = UnderflowIndex;
    return underflow;
  }

  static BinIndex Overflow() {
    BinIndex overflow;
    overflow.fIndex = OverflowIndex;
    return overflow;
  }
};

} // namespace EPHist

#endif
