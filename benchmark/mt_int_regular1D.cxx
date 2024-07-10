// SPDX-License-Identifier: LGPL-3.0-or-later

#include <EPHist/EPHist.hxx>
#include <EPHist/FillContext.hxx>
#include <EPHist/ParallelFillStrategy.hxx>
#include <EPHist/ParallelHelper.hxx>

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <random>
#include <thread>
#include <vector>

static constexpr std::size_t NumNumbers = 128 * 1024 * 1024;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Arguments: bins fills threads <mode> <distribution>\n");
    return 1;
  }

  const long bins = std::atol(argv[1]);
  const long fills = std::atol(argv[2]);
  const int threads = std::atoi(argv[3]);
  const long fillsPerThread = fills / threads;
  printf("%ld bins, %ld fills, %d threads\n", bins, fills, threads);

  // mode = 0: histograms for each thread + Fill, merge sequentially with Add
  // mode = 1: histograms for each thread + Fill, merge with AddAtomic
  // mode = 2: local histogram per thread + Fill, merge with AddAtomic
  // mode = 3: one histogram with FillAtomic
  // mode = 10: ParallelHelper + FillContext per thread (Automatic strategy)
  // mode = 11: ParallelHelper + FillContext per thread (Atomic strategy)
  // mode = 12: ParallelHelper + FillContext per thread (PerFillContext strat.)
  int mode = 10;
  if (argc > 4) {
    mode = std::atoi(argv[4]);
  }
  static const char *Modes[] = {"Fill_Add",
                                "Fill_AddAtomic (vector)",
                                "Fill_AddAtomic",
                                "FillAtomic",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                "ParallelHelper (Automatic)",
                                "ParallelHelper (Atomic)",
                                "ParallelHelper (PerFillContext)"};
  // distribution = 0: single value across all threads
  // distribution = 1: blocks of equidistributed values, to minimize collisions
  // distribution = 2: uniform distribution
  // distribution = 3: normal distribution, mean = 0.5, stddev = 0.25
  int distribution = 3;
  if (argc > 5) {
    distribution = std::atoi(argv[5]);
  }
  static const char *Distributions[] = {"single", "thread", "uniform",
                                        "normal"};
  printf("mode = %d = '%s', distribution = %d = '%s'\n", mode, Modes[mode],
         distribution, Distributions[distribution]);

  // First prepare an array of numbers outside of the timed section. We do this
  // even for the single values to generate the same memory traffic.
  std::vector<double> numbers(NumNumbers);
  if (distribution == 0) {
    for (std::size_t i = 0; i < numbers.size(); i++) {
      numbers[i] = 0.5;
    }
  } else if (distribution == 1) {
    const std::size_t numbersPerThread = NumNumbers / threads;
    for (std::size_t i = 0; i < numbers.size(); i++) {
      numbers[i] = (i / numbersPerThread + 0.5) / threads;
    }
  } else if (distribution == 2) {
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
    for (std::size_t i = 0; i < numbers.size(); i++) {
      numbers[i] = dis(gen);
    }
  } else if (distribution == 3) {
    std::mt19937 gen;
    std::normal_distribution<> dis(/*mean=*/0.5, /*stddev=*/0.25);
    for (std::size_t i = 0; i < numbers.size(); i++) {
      numbers[i] = dis(gen);
    }
  }

  auto start = std::chrono::steady_clock::now();

  if (mode == 0) {
    std::vector<EPHist::EPHist<int>> hists;
    hists.reserve(threads);
    auto callFill = [&](unsigned int t) {
      std::size_t numberOffset = t * NumNumbers / threads;
      EPHist::EPHist<int> &h1 = hists[t];
      for (long i = 0; i < fillsPerThread; i++) {
        h1.Fill(numbers[(numberOffset + i) % NumNumbers]);
      }
    };

    std::vector<std::thread> threadsV(threads);
    for (unsigned int t = 0; t < threads; t++) {
      hists.emplace_back(bins, 0.0, 1.0);
      threadsV[t] = std::thread(callFill, t);
    }

    for (auto &t : threadsV) {
      t.join();
    }

    // Merge the partial histograms.
    EPHist::EPHist<int> &h1 = hists[0];
    for (unsigned int t = 1; t < threads; t++) {
      h1.Add(hists[t]);
    }
  } else if (mode == 1) {
    std::vector<EPHist::EPHist<int>> hists;
    hists.reserve(threads + 1);
    auto callFill = [&](unsigned int t) {
      std::size_t numberOffset = t * NumNumbers / threads;
      EPHist::EPHist<int> &h1 = hists[t + 1];
      for (long i = 0; i < fillsPerThread; i++) {
        h1.Fill(numbers[(numberOffset + i) % NumNumbers]);
      }
      hists[0].AddAtomic(h1);
    };

    std::vector<std::thread> threadsV(threads);
    hists.emplace_back(bins, 0.0, 1.0);
    for (unsigned int t = 0; t < threads; t++) {
      hists.emplace_back(bins, 0.0, 1.0);
      threadsV[t] = std::thread(callFill, t);
    }

    for (auto &t : threadsV) {
      t.join();
    }
  } else if (mode == 2) {
    EPHist::EPHist<int> h1(bins, 0.0, 1.0);
    auto callFill = [&](unsigned int t) {
      std::size_t numberOffset = t * NumNumbers / threads;
      EPHist::EPHist<int> hLocal(bins, 0.0, 1.0);
      for (long i = 0; i < fillsPerThread; i++) {
        hLocal.Fill(numbers[(numberOffset + i) % NumNumbers]);
      }
      h1.AddAtomic(hLocal);
    };

    std::vector<std::thread> threadsV(threads);
    for (unsigned int t = 0; t < threads; t++) {
      threadsV[t] = std::thread(callFill, t);
    }

    for (auto &t : threadsV) {
      t.join();
    }
  } else if (mode == 3) {
    EPHist::EPHist<int> h1(bins, 0.0, 1.0);
    auto callFillAtomic = [&](unsigned int t) {
      std::size_t numberOffset = t * NumNumbers / threads;
      for (long i = 0; i < fillsPerThread; i++) {
        h1.FillAtomic(numbers[(numberOffset + i) % NumNumbers]);
      }
    };

    std::vector<std::thread> threadsV(threads);
    for (unsigned int t = 0; t < threads; t++) {
      threadsV[t] = std::thread(callFillAtomic, t);
    }

    for (auto &t : threadsV) {
      t.join();
    }
  } else if (mode >= 10) {
    const auto strategy = EPHist::ParallelFillStrategy(mode - 10);

    auto h1 = std::make_shared<EPHist::EPHist<int>>(bins, 0.0, 1.0);
    EPHist::ParallelHelper helper(h1, strategy);
    auto callFill = [&](unsigned int t) {
      auto context = helper.CreateFillContext();
      std::size_t numberOffset = t * NumNumbers / threads;
      for (long i = 0; i < fillsPerThread; i++) {
        context->Fill(numbers[(numberOffset + i) % NumNumbers]);
      }
    };

    std::vector<std::thread> threadsV(threads);
    for (unsigned int t = 0; t < threads; t++) {
      threadsV[t] = std::thread(callFill, t);
    }

    for (auto &t : threadsV) {
      t.join();
    }
  }

  auto end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> duration = end - start;

  double fillsPerThreadAndSecond = ((double)fillsPerThread) / duration.count();
  double fillsPerSecond = ((double)fills) / duration.count();
  printf(" === total time: %f s, fill frequency: %f MHz ===\n",
         duration.count(), fillsPerSecond / 1e6);
  printf(" === fill frequency per thread: %f MHz ===\n",
         fillsPerThreadAndSecond / 1e6);

  return 0;
}
