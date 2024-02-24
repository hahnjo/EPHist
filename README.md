# Efficient & Parallel Histograms

> [!CAUTION]
> This is a research prototype and not production-level code.
> *DO NOT* use for real applications and analyses.

## Design Goals

1. Make the common case fast: few dimensions with integer or floating point bins.
2. Template as little if possible, only if required to not lose performance.
3. Offer modern interfaces that are easy to use correctly.
4. Integrate methods for parallel filling, but without losing efficient sequential filling.
