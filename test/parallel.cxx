// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/ParallelFillStrategy.hxx>
#include <EPHist/ParallelHelper.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/Weight.hxx>

#include <gtest/gtest.h>

// Compatibility with GoogleTest < 1.10.0
#ifndef INSTANTIATE_TEST_SUITE_P
#define INSTANTIATE_TEST_SUITE_P INSTANTIATE_TEST_CASE_P
#endif

#include <memory>
#include <string>
#include <tuple>

static constexpr EPHist::ParallelFillStrategy kAllStrategies[] = {
    EPHist::ParallelFillStrategy::Automatic,
    EPHist::ParallelFillStrategy::Atomic,
    EPHist::ParallelFillStrategy::PerFillContext};
static std::string PrintStrategy(
    const testing::TestParamInfo<EPHist::ParallelFillStrategy> &info) {
  switch (info.param) {
  case EPHist::ParallelFillStrategy::Automatic:
    return "Automatic";
  case EPHist::ParallelFillStrategy::Atomic:
    return "Atomic";
  case EPHist::ParallelFillStrategy::PerFillContext:
    return "PerFillContext";
  }
  abort();
}

class ParallelHelperIntRegular1D
    : public testing::TestWithParam<EPHist::ParallelFillStrategy> {};

TEST_P(ParallelHelperIntRegular1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);
  EPHist::ParallelHelper helper(h1, GetParam());
}

TEST_P(ParallelHelperIntRegular1D, CreateFillContext) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);
  EPHist::ParallelHelper helper(h1, GetParam());

  auto context1 = helper.CreateFillContext();
  auto context2 = helper.CreateFillContext();
}

TEST_P(ParallelHelperIntRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(i);
    }
    context->Fill(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperIntRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(std::make_tuple(-100));
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(std::make_tuple(i));
    }
    context->Fill(std::make_tuple(100));
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperIntRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill<EPHist::RegularAxis>(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(i);
    }
    context->Fill<EPHist::RegularAxis>(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

INSTANTIATE_TEST_SUITE_P(Strategies, ParallelHelperIntRegular1D,
                         testing::ValuesIn(kAllStrategies), PrintStrategy);

class ParallelHelperLongLongRegular1D
    : public testing::TestWithParam<EPHist::ParallelFillStrategy> {};

TEST_P(ParallelHelperLongLongRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(i);
    }
    context->Fill(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperLongLongRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(std::make_tuple(-100));
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(std::make_tuple(i));
    }
    context->Fill(std::make_tuple(100));
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperLongLongRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill<EPHist::RegularAxis>(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(i);
    }
    context->Fill<EPHist::RegularAxis>(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_EQ(h1->GetBinContent(i), 1);
  }
}

INSTANTIATE_TEST_SUITE_P(Strategies, ParallelHelperLongLongRegular1D,
                         testing::ValuesIn(kAllStrategies), PrintStrategy);

class ParallelHelperFloatRegular1D
    : public testing::TestWithParam<EPHist::ParallelFillStrategy> {};

TEST_P(ParallelHelperFloatRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(i);
    }
    context->Fill(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperFloatRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(std::make_tuple(-100));
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(std::make_tuple(i));
    }
    context->Fill(std::make_tuple(100));
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperFloatRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill<EPHist::RegularAxis>(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(i);
    }
    context->Fill<EPHist::RegularAxis>(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperFloatRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST_P(ParallelHelperFloatRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST_P(ParallelHelperFloatRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

INSTANTIATE_TEST_SUITE_P(Strategies, ParallelHelperFloatRegular1D,
                         testing::ValuesIn(kAllStrategies), PrintStrategy);

class ParallelHelperDoubleRegular1D
    : public testing::TestWithParam<EPHist::ParallelFillStrategy> {};

TEST_P(ParallelHelperDoubleRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(i);
    }
    context->Fill(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperDoubleRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(std::make_tuple(-100));
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(std::make_tuple(i));
    }
    context->Fill(std::make_tuple(100));
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperDoubleRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill<EPHist::RegularAxis>(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(i);
    }
    context->Fill<EPHist::RegularAxis>(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i), 1);
  }
}

TEST_P(ParallelHelperDoubleRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST_P(ParallelHelperDoubleRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST_P(ParallelHelperDoubleRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

INSTANTIATE_TEST_SUITE_P(Strategies, ParallelHelperDoubleRegular1D,
                         testing::ValuesIn(kAllStrategies), PrintStrategy);

class ParallelHelperDoubleBinWithErrorRegular1D
    : public testing::TestWithParam<EPHist::ParallelFillStrategy> {};

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(i);
    }
    context->Fill(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    auto &binWithError = h1->GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill(std::make_tuple(-100));
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(std::make_tuple(i));
    }
    context->Fill(std::make_tuple(100));
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    auto &binWithError = h1->GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    context->Fill<EPHist::RegularAxis>(-100);
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(i);
    }
    context->Fill<EPHist::RegularAxis>(100);
  }

  for (std::size_t i = 0; i < h1->GetTotalNumBins(); i++) {
    auto &binWithError = h1->GetBinContent(i);
    EXPECT_FLOAT_EQ(binWithError.fSum, 1);
    EXPECT_FLOAT_EQ(binWithError.fSum2, 1);
  }
}

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1->GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1->GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

TEST_P(ParallelHelperDoubleBinWithErrorRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1, GetParam());
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    auto &binWithError = h1->GetBinContent(i + 1);
    double weight = 0.5 + i * 0.1;
    EXPECT_FLOAT_EQ(binWithError.fSum, weight);
    EXPECT_FLOAT_EQ(binWithError.fSum2, weight * weight);
  }
}

INSTANTIATE_TEST_SUITE_P(Strategies, ParallelHelperDoubleBinWithErrorRegular1D,
                         testing::ValuesIn(kAllStrategies), PrintStrategy);
