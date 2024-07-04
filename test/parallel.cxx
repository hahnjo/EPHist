// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/ParallelFillStrategy.hxx>
#include <EPHist/ParallelHelper.hxx>
#include <EPHist/RegularAxis.hxx>
#include <EPHist/Weight.hxx>

#include <gtest/gtest.h>

#include <memory>
#include <tuple>

TEST(ParallelHelperIntRegular1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);
  { EPHist::ParallelHelper helper(h1); }

  {
    EPHist::ParallelHelper helperAuto(h1,
                                      EPHist::ParallelFillStrategy::Automatic);
  }

  {
    EPHist::ParallelHelper helperAtomic(h1,
                                        EPHist::ParallelFillStrategy::Atomic);
  }
}

TEST(ParallelHelperIntRegular1D, CreateFillContext) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);
  EPHist::ParallelHelper helper(h1);

  auto context1 = helper.CreateFillContext();
  auto context2 = helper.CreateFillContext();
}

TEST(ParallelHelperIntRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperIntRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperIntRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<int>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperLongLongRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperLongLongRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperLongLongRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<long long>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperFloatRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperFloatRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperFloatRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperFloatRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperFloatRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperFloatRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<float>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperDoubleRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperDoubleRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill(EPHist::Weight(0.5 + i * 0.1), std::make_tuple(i));
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperDoubleRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<double>>(Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
    auto context = helper.CreateFillContext();
    for (std::size_t i = 0; i < Bins; i++) {
      context->Fill<EPHist::RegularAxis>(EPHist::Weight(0.5 + i * 0.1), i);
    }
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_FLOAT_EQ(h1->GetBinContent(i + 1), 0.5 + i * 0.1);
  }
}

TEST(ParallelHelperDoubleBinWithErrorRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleBinWithErrorRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleBinWithErrorRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleBinWithErrorRegular1D, FillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleBinWithErrorRegular1D, FillTupleWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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

TEST(ParallelHelperDoubleBinWithErrorRegular1D, TemplatedFillWeight) {
  static constexpr std::size_t Bins = 20;
  auto h1 = std::make_shared<EPHist::EPHist<EPHist::DoubleBinWithError>>(
      Bins, 0, Bins);

  {
    EPHist::ParallelHelper helper(h1);
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
