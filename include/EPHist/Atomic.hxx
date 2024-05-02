// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_ATOMIC
#define EPHIST_ATOMIC

#include <type_traits>

namespace EPHist {
namespace Internal {

// A simple version of the functionality provided by C++20 std::atomic_ref.

// AtomicAddDouble is not needed for integral types.
template <typename T>
std::enable_if_t<std::is_integral_v<T>> AtomicInc(T *ptr) {
  __atomic_fetch_add(ptr, 1, __ATOMIC_RELAXED);
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>> AtomicAdd(T *ptr, T add) {
  T expected;
  __atomic_load(ptr, &expected, __ATOMIC_RELAXED);
  T desired = expected + add;
  while (!__atomic_compare_exchange(ptr, &expected, &desired, /*weak=*/false,
                                    __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
    // expected holds the new value; try again.
    desired = expected + add;
  }
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>> AtomicAddDouble(T *ptr,
                                                              double add) {
  AtomicAdd(ptr, static_cast<T>(add));
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>> AtomicInc(T *ptr) {
  AtomicAdd(ptr, static_cast<T>(1));
}

template <typename T>
std::enable_if_t<
    std::is_member_function_pointer_v<decltype(&T::AtomicAddDouble)>>
AtomicAddDouble(T *ptr, double add) {
  ptr->AtomicAddDouble(add);
}

template <typename T>
std::enable_if_t<std::is_member_function_pointer_v<decltype(&T::AtomicInc)>>
AtomicInc(T *ptr) {
  ptr->AtomicInc();
}

} // namespace Internal
} // namespace EPHist

#endif
