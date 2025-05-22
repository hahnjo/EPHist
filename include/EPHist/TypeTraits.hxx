// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_TYPETRAITS
#define EPHIST_TYPETRAITS

namespace EPHist {
namespace Internal {

template <typename T, typename... Ts> struct LastType : LastType<Ts...> {};
template <typename T> struct LastType<T> {
  using type = T;
};

} // namespace Internal
} // namespace EPHist

#endif
