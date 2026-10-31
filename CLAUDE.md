# CLAUDE.md

## What this is

A C++26 test bench for PRNGs.  The program `prng-dump` writes raw output of a named PRNG to
stdout, and PractRand's `RNG_test` measures its randomness.  Speed is measured with Google
Benchmark by `prng-next-benchmark` (generating values) and `prng-construct-benchmark`
(constructing a randomly seeded PRNG).
There are no unit tests.  The "tests" are PractRand runs whose outputs are committed under
`results/`.

## Commands

The top-level `Makefile` checks for every program in `REQUIRED_BINS` (`RNG_test`, parallel,
datamash, jq, ...) at parse time, so any `make` invocation fails if one is missing.
Only g++ is supported (clang++ is not).  The benchmark programs link with `-lbenchmark`
(Google Benchmark), which must be installed.

- `make` builds every top-level `*.cpp`: `prng-dump` (plus `prng-dump.opts`, the recorded
  compiler flags), `prng-next-benchmark`, `prng-construct-benchmark`, and `warmup-survey`.
- `make lint` runs clang-tidy using `.clang-tidy` on every top-level `*.cpp`.  Make ignores
  clang-tidy's exit status.
- `make benchmark` runs `run-benchmarks.bash`, which runs every `*-benchmark` program and
  writes `results/PROGRAM.json` (raw) and `results/PROGRAM.txt` (`name value unit`, from
  `results/filter-benchmark-results.jq`).  `prng-next-benchmark` reports GiB/s per thread,
  and `prng-construct-benchmark` reports ns per construction.  The script sets `NUM_THREADS`
  to one less than `nproc`, and `BENCHMARK_REPS` (default 5) sets the repetitions whose
  median is reported.
- `make short-test` runs `make benchmark`, then PractRand to 256MB for all four seed types
  (about 4 minutes in all).  The PractRand output goes to
  `results/prng-results.tlmax-256MB.summary.txt`, so that part prints nothing while it runs.
- `make long-test` runs `make short-test`, then PractRand to 512GB with random seeds
  (about 27.7 hours).  Its PractRand output goes to the `tlmax-512GB` summary file.  Start it
  only when the user asks for it.
- `make update-short-test` / `update-long-test` do a parallel `--dry-run` that regenerates
  the `files`, `names`, and JSON results from existing `RNG_test` outputs and newer
  benchmark data.  The summary goes to `*.summary.dry-run.txt`, so `*.summary.txt` keeps the
  real run's timings.
- `make -C include` compiles each `*.hpp` standalone with `-Werror` to check that it is
  self-contained.  Make ignores a failed compile and still exits 0, so read the output.  A
  `#warning` from an ISA guard does not fail a compile.  The `*.h` files are not checked.
  `make -C include wyrand.o` checks one header, and `make -C include lint` lints the headers.
- `make warmup-survey && ./warmup-survey` runs the warm-up survey (see **Warm-ups** below).

These commands work with one PRNG.

- `./prng-dump -i` lists PRNGs as tab-separated `name  result_bits  min  max  state_bytes`.
  Scripts parse these columns by position, so do not reorder them.
- `./prng-dump -s {default|pattern|random|zero} [-l GiB] NAME` dumps output.
- `bash test-prng-dump.bash -m 256MB -s zero NAME...` tests only the named PRNGs.  It
  overwrites the matching files in `results/`.

A manual run looks like this.

```sh
./prng-dump -s zero wyrand | RNG_test stdin64 -tlmax 256MB -multithreaded
```

`result_bits` is the number of uniformly random bits each call returns.  `prng-dump` writes
each value in a word of the largest power of 2 bits that is at most `result_bits`, and
`stdinN` has to match that word.  For example, `std::ranlux24` returns 24 bits, so it needs
`stdin16`.  `RNG_test` accepts at most `stdin64`, so a 128-bit PRNG also uses `stdin64`.

## Architecture

**PRNG headers (`include/*.hpp`).**  Every non-`std` PRNG derives from
`URBG_base<state_type, result_type>` in `urbg_base_class.hpp`, declared with the
`DEF_URBG_SUBCLASS(NAME, STATE, RESULT)` macro.  Wrap a state type containing a comma in
`SINGLE_ARG(...)`.  The header then defines `NAME::init()`, which fixes up the state (for
example, forcing an element odd or nonzero), and `NAME::next()`.  The protected member `s`
holds the state.  The macro gives three constructors:

- The default constructor fills the state from `arc4random_buf`, so it is *randomly* seeded.
  This differs from `std` engines, whose default constructor gives a fixed seed.
- One constructor takes a `state_type`.
- One takes `seed_bytes_type` (a byte array the size of the state).

Do not make `next()` or the destructor `virtual`.  Nothing holds a `URBG_base` pointer or
reference, and a virtual `next()` costs most PRNGs several times their speed.  The class
doc block in `urbg_base_class.hpp` carries the argument.

The remaining headers (`int_*`, `mum.hpp`, `clmum.hpp`, `simd_*`, `*primes*`, `sha2.h`, ...)
are shared building blocks.  Several are synced from the author's other repos.

**Warm-ups.**  Some `init()` functions discard their first outputs, because with the zero
seed those outputs have mostly zero bits.  For several PRNGs, the zero seed failed PractRand
immediately without the warm-up.  The xoshiro and xoroshiro families never leave an all-zero
state, so their `init()` first replaces it with 1, 2, 3, and so on.

`warmup-survey` starts each such PRNG from the state `init()` has before the warm-up and
reports how many outputs to discard before they look filled, followed by the per-call
percentages behind each count.  Its list in `main()` must start each PRNG from zeros
(`zero_start`) or from 1, 2, 3, and so on (`iota_start`), matching its `init()`.  The counts
(`num_warmup_discards`) are set by hand and roughly match the survey's popcount column.
`biski64` is the exception, since it keeps the 16 from upstream's `biski64_warmup`.  The
survey does not read the counts, so when a warm-up or a zero-seed fix-up changes, rerun it
and update the count by hand.

**Registration has five places that must stay in sync.**  Adding a PRNG takes these steps.

1. Include its header in `include/prng.hpp`.
2. Add `CREATE_PRNG_INFO_MAP_ENTRY(name)` to `prng_name_to_info` in `include/prng.hpp`.
3. Add `CONDITIONAL_DUMP(name)` to `main` in `prng-dump.cpp`.
4. Add `REGISTER_BENCHMARK_PRNG_NEXT(name)` to `main` in `prng-next-benchmark.cpp`.
5. Add `REGISTER_BENCHMARK_PRNG_CONSTRUCT(name)` to `main` in `prng-construct-benchmark.cpp`.

If `init()` has a warm-up, also add the PRNG to the list in `main()` of `warmup-survey.cpp`.

Keep the existing order (mostly alphabetical) and column alignment.  Guard ISA-dependent
PRNGs with the same `#if defined(__AES__)` / `__PCLMUL__` / `__SHA__` in all of these files,
and wrap the PRNG's own header in that guard too (with a `#warning` in the `#else`, as
`aes_ctr.hpp` does).  `prng.hpp` includes every header, so an unguarded header breaks the
build on a CPU target that lacks the instruction set.  The Makefile builds with `-march=native`,
so which guarded PRNGs exist depends on the CPU of the machine that builds them.

**Benchmarks.**  `prng-benchmark.hpp` holds what the two benchmark programs share.  Its
`make_random_seeded<URBG>()` seeds a `std` engine with `random_device_seeded` and one of the
repo's PRNGs (detected by `my_urbg`) with its default constructor, so one benchmark function
serves both.  `get_num_threads()` in `get_num_threads.hpp` reads `NUM_THREADS`, where 0 means
every hardware thread.  A benchmark program run without `NUM_THREADS` set uses one thread.

**Seed types.**  The two `make_seeded` overloads in `prng-dump.cpp` turn a seed type into a
seeded PRNG.  They tell the repo's own PRNGs from `std` engines with the `my_urbg` concept in
`urbg_base_class.hpp`.  For `std` engines, seeding goes through the seed sequences in
`seed_seq.hpp` (`fill_seed_seq`, `random_device_seeded`).  For the repo's own PRNGs, `pattern`
and `zero` pass byte arrays, while `default` and `random` both use the default constructor.
`test-prng-dump.bash` therefore diffs the default and random pass/fail lists as a sanity check.

**Results pipeline.**  `test-prng-dump.bash` generates one
`prng-dump | RNG_test > results/RNG_test.tlmax-X.seed-Y.prng-NAME.txt` command per PRNG and
runs them with GNU parallel.  A run fails if its output contains `FAIL`.  The good and failed
file names go to `prng-results.*.files.{good,failed}.txt`, and the PRNG names go to
`prng-results.*.names.{good,failed}.txt`.  A file with no `length=` line (an empty or cut-off
run) is in neither list, and the script warns about it.  Each PRNG's test length and result
are joined with `prng-next-benchmark.txt` and converted by `results/parse-prng-results.jq` into
`prng-results.*.json`, which `results/plot-results.py` plots.

## Conventions

@COMMENT-STYLE.md

- Code files carry `SPDX-FileCopyrightText` / `SPDX-License-Identifier` headers.  A new file
  takes MPL-2.0, and a header adapted from upstream keeps upstream's license.
- Brace the body of every `for`, `if`, `while`, and `else`, even a single statement, with each
  brace on its own line.  This holds inside multi-line macros too.  `.clang-tidy` disables the
  braces checks, so lint does not catch a missing brace.
- `prng-dump.cpp` (`program_version`) and `results/plot-results.py` (`__version__`) carry a
  date version.  After editing one, set it to today (YYYY-MM-DD) in a separate commit with the
  subject "Update program version".
