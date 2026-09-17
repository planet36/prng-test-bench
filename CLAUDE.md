# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A C++26 test bench for PRNGs.  A single program, `prng-dump`, writes raw output of a named
PRNG to stdout.  Speed is measured with hyperfine and randomness with PractRand's `RNG_test`.
There are no unit tests.  The "tests" are PractRand runs whose outputs are committed under
`results/`.

## Commands

The top-level `Makefile` checks for every program in `REQUIRED_BINS` (hyperfine, `RNG_test`,
parallel, datamash, jq, ...) at parse time, so any `make` invocation fails if one is missing.
Only g++ is supported (clang++ is not).  The `fmt` library is required to link.

- `make` builds `prng-dump` (plus `prng-dump.opts`, the recorded compiler flags).
- `make lint` runs clang-tidy using `.clang-tidy`.
- `make prng-bench` benchmarks every PRNG (~9 min) and writes `results/prng-bench.{csv,txt}`.
- `make short-test` runs PractRand to 256MB for all four seed types (~11 min).
- `make long-test` runs PractRand to 512GB with random seeds (~40 hours).  Do not start this
  casually.
- `make update-short-test` / `update-long-test` do a parallel `--dry-run` that regenerates
  the summary/JSON files from existing `RNG_test` outputs and newer benchmark data.
- `make -C include` compiles each header standalone (as `-Werror`) to check that it is
  self-contained; `make -C include wyrand.o` checks one header, and `make -C include lint`
  lints the headers.
- `warmup-survey.cpp` is not built by `make`.  Build and run it with
  `g++ -std=c++26 -O2 -march=native -I include warmup-survey.cpp -o warmup-survey && ./warmup-survey`.
  It checks the warm-up counts (outputs discarded in `init()`), which were picked by eye as
  the zero-seed outputs that still look empty.  Those counts roughly match discarding outputs
  until the first one with at least 40% of its bits set.  The warm-ups, and the replacement
  of an all-zero state with 1, 2, 3, ..., exist because those PRNGs otherwise failed PractRand
  immediately with the zero seed.

Working with one PRNG:

- `./prng-dump -i` lists PRNGs as tab-separated `name  result_bits  min  max  state_bytes`.
  Scripts parse these columns by position, so do not reorder them.
- `./prng-dump -s {default|pattern|random|zero} [-l GiB] NAME` dumps output.
- `bash test-prng-dump.bash -m 256MB -s zero NAME...` tests only the named PRNGs.  It
  overwrites the matching files in `results/`.
- A manual run looks like `./prng-dump -s zero wyrand | RNG_test stdin64 -tlmax 256MB -multithreaded`.
  `RNG_test` accepts at most `stdin64`.

## Architecture

**PRNG headers (`include/*.hpp`).**  Every non-`std` PRNG derives from
`AbstractURBG<state_type, result_type>` in `abstract_urbg_class.hpp`, declared with the
`DEF_URBG_SUBCLASS(NAME, STATE, RESULT)` macro.  Wrap a state type containing a comma in
`SINGLE_ARG(...)`.  The header then defines `NAME::init()`, which fixes up the state (for
example, forcing an element odd or nonzero), and `NAME::next()`.  The protected member `s`
holds the state.  The macro gives three constructors:

- The default constructor fills the state from `arc4random_buf`, so it is *randomly* seeded.
  This differs from `std` engines, whose default constructor gives a fixed seed.
- One constructor takes a `state_type`.
- One takes `seed_bytes_type` (a byte array the size of the state).

The remaining headers (`int_*`, `mum*`, `simd_*`, `*primes*`, `sha2_iv.h`, ...) are shared
building blocks.  Several are synced from the author's other repos.

**Registration has three places that must stay in sync.**  To add a PRNG:

1. Include its header in `include/prng.hpp`.
2. Add `CREATE_PRNG_INFO_MAP_ENTRY(name)` to `prng_name_to_info` in `include/prng.hpp`.
3. Add `CONDITIONAL_DUMP_MINE(name)` to `main` in `prng-dump.cpp`.

Keep the alphabetical order and column alignment.  Guard ISA-dependent PRNGs with the same
`#if defined(__AES__)` / `__PCLMUL__` / `__SHA__` in both files.

**Seed types.**  For `std` engines, seeding goes through the seed sequences in
`seed_seq.hpp` (`fill_seed_seq`, `random_device_seeded`).  For the repo's own PRNGs, `pattern`
and `zero` pass byte arrays, while `default` and `random` both use the default constructor.
`test-prng-dump.bash` therefore diffs the default and random pass/fail lists as a sanity check.

**Results pipeline.**  `test-prng-dump.bash` generates one
`prng-dump | RNG_test > results/RNG_test.tlmax-X.seed-Y.prng-NAME.txt` command per PRNG and
runs them with GNU parallel.  A run fails if its output contains `FAIL`.  Good and failed
names are written to `prng-results.*.{files,names}.{good,failed}.txt`.  They are joined with
`prng-bench.txt` and converted by `results/filter.jq` into `prng-results.*.json`, which
`results/plot-results.py` plots.

## Conventions

- `COMMENT-STYLE.md` governs code comments, Doxygen blocks, and commit messages.  Follow it
  when writing any of those (for example: two spaces after periods, 96-column limit, Doxygen
  briefs on `///` lines with no terminating period, `\a` for parameters).
- Files carry `SPDX-FileCopyrightText` / `SPDX-License-Identifier` headers (MPL-2.0 for C++).
- Commit subjects are short imperative sentences without a trailing period.
