// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef EPHIST_PROFILE
#define EPHIST_PROFILE

#include "Axes.hxx"
#include "TypeTraits.hxx"
#include "Weight.hxx"

#include <vector>

namespace EPHist {

template <bool WithError = true> class Profile final {
public:
  struct DoubleBin {
    double fSumValues = 0;
    double fSumValues2 = 0;
    // We could allow a different type here, but a smaller type would not reduce
    // memory because the struct has to be padded to the width of a double. The
    // only interesting type would be a 64-bit integer, but always using double
    // simplifies the implementation because weighted Fills are always allowed.
    double fSum = 0;

    DoubleBin &operator+=(const DoubleBin &rhs) {
      fSumValues += rhs.fSumValues;
      fSumValues2 += rhs.fSumValues2;
      fSum += rhs.fSum;
      return *this;
    }

    void Add(double v) {
      fSumValues += v;
      fSumValues2 += v * v;
      fSum++;
    }

    void Add(double v, double w) {
      fSumValues += w * v;
      fSumValues2 += w * v * v;
      fSum += w;
    }
  };

  struct DoubleBinWithError {
    double fSumValues = 0;
    double fSumValues2 = 0;
    double fSum = 0;
    double fSum2 = 0;

    DoubleBinWithError &operator+=(const DoubleBinWithError &rhs) {
      fSumValues += rhs.fSumValues;
      fSumValues2 += rhs.fSumValues2;
      fSum += rhs.fSum;
      fSum2 += rhs.fSum2;
      return *this;
    }

    void Add(double v) {
      fSumValues += v;
      fSumValues2 += v * v;
      fSum++;
      fSum2++;
    }

    void Add(double v, double w) {
      fSumValues += w * v;
      fSumValues2 += w * v * v;
      fSum += w;
      fSum2 += w * w;
    }
  };

  using BinContentType =
      std::conditional_t<WithError, DoubleBinWithError, DoubleBin>;

private:
  std::vector<BinContentType> fData;

  Detail::Axes fAxes;

public:
  explicit Profile(const std::vector<AxisVariant> &axes) : fAxes(axes) {
    fData.resize(fAxes.ComputeTotalNumBins());
  }

  // Copy constructor and assignment operator are deleted to avoid surprises.
  // Use the explicit Clone() function to create a copy.
  Profile(const Profile<WithError> &) = delete;
  Profile(Profile<WithError> &&) = default;
  Profile<WithError> &operator=(const Profile<WithError> &) = delete;
  Profile<WithError> &operator=(Profile<WithError> &&) = default;
  ~Profile() = default;

  void Add(const Profile<WithError> &other) {
    if (fAxes != other.fAxes) {
      throw std::invalid_argument("axes configuration not identical");
    }
    for (std::size_t i = 0; i < fData.size(); i++) {
      fData[i] += other.fData[i];
    }
  }

  void Clear() {
    for (std::size_t i = 0; i < fData.size(); i++) {
      fData[i] = {};
    }
  }

  Profile<WithError> Clone() {
    Profile<WithError> p(fAxes.GetVector());
    for (std::size_t i = 0; i < fData.size(); i++) {
      p.fData[i] += fData[i];
    }
    return p;
  }

  const BinContentType &GetBinContent(std::size_t bin) const {
    assert(bin >= 0 && bin < fData.size());
    return fData[bin];
  }
  template <std::size_t N>
  const BinContentType &
  GetBinContentAt(const std::array<BinIndex, N> &args) const {
    if (N != fAxes.GetNumDimensions()) {
      throw std::invalid_argument(
          "invalid number of arguments to GetBinContent");
    }
    auto bin = fAxes.ComputeBin(args);
    if (!bin.second) {
      throw std::invalid_argument("bin not found");
    }
    return fData[bin.first];
  }
  template <typename... A>
  const BinContentType &GetBinContentAt(const A &...args) const {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument(
          "invalid number of arguments to GetBinContent");
    }
    std::array<BinIndex, sizeof...(A)> a{args...};
    return GetBinContentAt(a);
  }
  std::size_t GetTotalNumBins() const { return fData.size(); }

  const std::vector<AxisVariant> &GetAxes() const { return fAxes.GetVector(); }
  std::size_t GetNumDimensions() const { return fAxes.GetNumDimensions(); }

private:
  template <std::size_t N, typename... A>
  void FillImpl(const std::tuple<A...> &args, double v) {
    assert(N == fAxes.GetNumDimensions());
    auto bin = fAxes.ComputeBin<N>(args);
    if (bin.second) {
      fData[bin.first].Add(v);
    }
  }

  template <std::size_t N, typename... A>
  void FillImpl(const std::tuple<A...> &args, double v, Weight w) {
    assert(N == fAxes.GetNumDimensions());
    auto bin = fAxes.ComputeBin<N>(args);
    if (bin.second) {
      fData[bin.first].Add(v, w.fValue);
    }
  }

public:
  template <typename... A> void Fill(const std::tuple<A...> &args) {
    if (sizeof...(A) - 1 != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A) - 1>(args, std::get<sizeof...(A) - 1>(args));
  }

  // We have to accept v with a template type V to capture any argument,
  // otherwise Fill(std::tuple<int>, int) would select the variadic function
  // template...
  template <typename... A, typename V>
  void Fill(const std::tuple<A...> &args, const V &v) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A)>(args, v);
  }

  template <typename... A> void Fill(const A &...args) {
    auto t = std::forward_as_tuple(args...);
    // Could use std::tuple_element_t<sizeof...(A) - 1, decltype(t)>, but that
    // would be const Weight &
    if constexpr (std::is_same_v<typename Internal::LastType<A...>::type,
                                 Weight>) {
      if (sizeof...(A) - 2 != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      FillImpl<sizeof...(A) - 2>(t, std::get<sizeof...(A) - 2>(t),
                                 std::get<sizeof...(A) - 1>(t));
    } else {
      if (sizeof...(A) - 1 != fAxes.GetNumDimensions()) {
        throw std::invalid_argument("invalid number of arguments to Fill");
      }
      Fill(t);
    }
  }

  template <typename... A> void Fill(const std::tuple<A...> &args, Weight w) {
    if (sizeof...(A) - 1 != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A) - 1>(args, std::get<sizeof...(A) - 1>(args), w);
  }

  // We have to accept v with a template type V to capture any argument,
  // otherwise Fill(std::tuple<int>, int) would select the variadic function
  // template...
  template <typename... A, typename V>
  void Fill(const std::tuple<A...> &args, const V &v, Weight w) {
    if (sizeof...(A) != fAxes.GetNumDimensions()) {
      throw std::invalid_argument("invalid number of arguments to Fill");
    }
    FillImpl<sizeof...(A)>(args, v, w);
  }
};

} // namespace EPHist

#endif
