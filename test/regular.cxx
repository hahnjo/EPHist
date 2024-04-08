// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/RegularAxis.hxx>

#include <gtest/gtest.h>

#include <tuple>
#include <variant>

TEST(RegularAxis, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EXPECT_EQ(axis.GetNumBins(), Bins);
  EXPECT_EQ(axis.GetLow(), 0);
  EXPECT_EQ(axis.GetHigh(), Bins);
}

TEST(RegularAxis, Equality) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axisA(Bins, 0, Bins);
  EPHist::RegularAxis axisB(Bins, 0, Bins);
  EPHist::RegularAxis axisC(Bins / 2, 0, Bins);
  EPHist::RegularAxis axisD(Bins, 0, Bins / 2);
  EPHist::RegularAxis axisE(Bins, Bins / 2, Bins);

  EXPECT_TRUE(axisA == axisA);
  EXPECT_TRUE(axisA == axisB);
  EXPECT_TRUE(axisB == axisA);

  EXPECT_FALSE(axisA == axisC);
  EXPECT_FALSE(axisA == axisD);
  EXPECT_FALSE(axisA == axisE);

  EXPECT_FALSE(axisC == axisD);
  EXPECT_FALSE(axisC == axisE);

  EXPECT_FALSE(axisD == axisE);
  EXPECT_FALSE(axisE == axisD);
}

TEST(RegularAxis, ComputeBin) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(axis.ComputeBin(i + 0.5), i);
  }
}

TEST(IntRegular1D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);
  EXPECT_EQ(h1.GetNumBins(), Bins);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  const auto &axes = h1.GetAxes();
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 0);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::RegularAxis>(axes[0]).GetNumBins(), Bins);

  EPHist::RegularAxis axis(2 * Bins, 0, Bins);
  h1 = EPHist::EPHist<int>(axis);
  EXPECT_EQ(h1.GetNumBins(), 2 * Bins);
  EXPECT_EQ(h1.GetNumDimensions(), 1);
  ASSERT_EQ(axes.size(), 1);
  EXPECT_EQ(axes[0].index(), 0);
  ASSERT_TRUE(std::get_if<EPHist::RegularAxis>(&axes[0]) != nullptr);
  EXPECT_EQ(std::get<EPHist::RegularAxis>(axes[0]).GetNumBins(), 2 * Bins);
}

TEST(IntRegular1D, Fill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, FillTuple) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill(std::make_tuple(i));
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular1D, TemplatedFill) {
  static constexpr std::size_t Bins = 20;
  EPHist::EPHist<int> h1(Bins, 0, Bins);

  for (std::size_t i = 0; i < Bins; i++) {
    h1.Fill<EPHist::RegularAxis>(i);
  }

  for (std::size_t i = 0; i < Bins; i++) {
    EXPECT_EQ(h1.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, Constructor) {
  static constexpr std::size_t Bins = 20;
  EPHist::RegularAxis axis(Bins, 0, Bins);
  EPHist::EPHist<int> h2({axis, axis});
  EXPECT_EQ(h2.GetNumBins(), Bins * Bins);
  EXPECT_EQ(h2.GetNumDimensions(), 2);
  const auto &axes = h2.GetAxes();
  EXPECT_EQ(axes.size(), 2);

  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  h2 = EPHist::EPHist<int>({axis, axisY});
  EXPECT_EQ(h2.GetNumBins(), Bins * BinsY);
  EXPECT_EQ(h2.GetNumDimensions(), 2);
  EXPECT_EQ(axes.size(), 2);
}

TEST(IntRegular2D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill(x, y);
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY; i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, FillTuple) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill(std::make_tuple(x, y));
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY; i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular2D, TemplatedFill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  EPHist::EPHist<int> h2({axisX, axisY});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      h2.Fill<EPHist::RegularAxis, EPHist::RegularAxis>(x, y);
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY; i++) {
    EXPECT_EQ(h2.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, Constructor) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});
  EXPECT_EQ(h3.GetNumBins(), BinsX * BinsY * BinsZ);
  EXPECT_EQ(h3.GetNumDimensions(), 3);
  const auto &axes = h3.GetAxes();
  EXPECT_EQ(axes.size(), 3);
}

TEST(IntRegular3D, Fill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      for (std::size_t z = 0; z < BinsZ; z++) {
        h3.Fill(x, y, z);
      }
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY * BinsZ; i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, FillTuple) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      for (std::size_t z = 0; z < BinsZ; z++) {
        h3.Fill(std::make_tuple(x, y, z));
      }
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY * BinsZ; i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular3D, TemplatedFill) {
  static constexpr std::size_t BinsX = 20;
  EPHist::RegularAxis axisX(BinsX, 0, BinsX);
  static constexpr std::size_t BinsY = 30;
  EPHist::RegularAxis axisY(BinsY, 0, BinsY);
  static constexpr std::size_t BinsZ = 50;
  EPHist::RegularAxis axisZ(BinsZ, 0, BinsZ);
  EPHist::EPHist<int> h3({axisX, axisY, axisZ});

  for (std::size_t x = 0; x < BinsX; x++) {
    for (std::size_t y = 0; y < BinsY; y++) {
      for (std::size_t z = 0; z < BinsZ; z++) {
        h3.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis>(
            x, y, z);
      }
    }
  }

  for (std::size_t i = 0; i < BinsX * BinsY * BinsZ; i++) {
    EXPECT_EQ(h3.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, Constructor) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});
  EXPECT_EQ(h4.GetNumBins(), Bins0 * Bins1 * Bins2 * Bins3);
  EXPECT_EQ(h4.GetNumDimensions(), 4);
  const auto &axes = h4.GetAxes();
  EXPECT_EQ(axes.size(), 4);
}

TEST(IntRegular4D, Fill) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (std::size_t v0 = 0; v0 < Bins0; v0++) {
    for (std::size_t v1 = 0; v1 < Bins1; v1++) {
      for (std::size_t v2 = 0; v2 < Bins2; v2++) {
        for (std::size_t v3 = 0; v3 < Bins3; v3++) {
          h4.Fill(v0, v1, v2, v3);
        }
      }
    }
  }

  for (std::size_t i = 0; i < Bins0 * Bins1 * Bins2 * Bins3; i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, FillTuple) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (std::size_t v0 = 0; v0 < Bins0; v0++) {
    for (std::size_t v1 = 0; v1 < Bins1; v1++) {
      for (std::size_t v2 = 0; v2 < Bins2; v2++) {
        for (std::size_t v3 = 0; v3 < Bins3; v3++) {
          h4.Fill(std::make_tuple(v0, v1, v2, v3));
        }
      }
    }
  }

  for (std::size_t i = 0; i < Bins0 * Bins1 * Bins2 * Bins3; i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}

TEST(IntRegular4D, TemplatedFill) {
  static constexpr std::size_t Bins0 = 10;
  EPHist::RegularAxis axis0(Bins0, 0, Bins0);
  static constexpr std::size_t Bins1 = 20;
  EPHist::RegularAxis axis1(Bins1, 0, Bins1);
  static constexpr std::size_t Bins2 = 30;
  EPHist::RegularAxis axis2(Bins2, 0, Bins2);
  static constexpr std::size_t Bins3 = 50;
  EPHist::RegularAxis axis3(Bins3, 0, Bins3);
  EPHist::EPHist<int> h4({axis0, axis1, axis2, axis3});

  for (std::size_t v0 = 0; v0 < Bins0; v0++) {
    for (std::size_t v1 = 0; v1 < Bins1; v1++) {
      for (std::size_t v2 = 0; v2 < Bins2; v2++) {
        for (std::size_t v3 = 0; v3 < Bins3; v3++) {
          h4.Fill<EPHist::RegularAxis, EPHist::RegularAxis, EPHist::RegularAxis,
                  EPHist::RegularAxis>(v0, v1, v2, v3);
        }
      }
    }
  }

  for (std::size_t i = 0; i < Bins0 * Bins1 * Bins2 * Bins3; i++) {
    EXPECT_EQ(h4.GetBinContent(i), 1);
  }
}
