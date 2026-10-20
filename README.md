# prng-test-bench

prng-test-bench measures the speed of PRNGs with [Google Benchmark](https://github.com/google/benchmark) and their randomness with [PractRand](https://github.com/planet36/PractRand).

## Building

Run `make` to build `prng-dump`, the benchmark programs (`prng-next-benchmark` and `prng-construct-benchmark`), and `warmup-survey`.

## Usage

### Running one PRNG

`prng-dump` writes the raw output of a PRNG to stdout:
- `./prng-dump -i` lists the available PRNGs, with columns for the name, result bits, minimum, maximum, and state bytes.
- `./prng-dump -s {default|pattern|random|zero} [-l GiB] NAME` dumps the output of one PRNG.
- `./prng-dump -h` prints the full usage.

This command tests one PRNG by hand.
```sh
./prng-dump -s zero wyrand | RNG_test stdin64 -tlmax 256MB -multithreaded
```

The second column of `./prng-dump -i` is the number of uniformly random bits each call returns.  `prng-dump` writes each value in a word of the largest power of 2 bits that is at most that number, and the `stdinN` given to `RNG_test` has to match that word.  For example, `std::ranlux24` returns 24 bits, so it needs `stdin16`.  `RNG_test` accepts at most `stdin64`, so a 128-bit PRNG also uses `stdin64`.

### Benchmarks

Run `make benchmark` to run only the benchmarks.
- It runs `run-benchmarks.bash`, which runs each `*-benchmark` program.
- `prng-next-benchmark` measures how fast each PRNG generates values (GiB/s per thread).
- `prng-construct-benchmark` measures how long it takes to construct a randomly seeded PRNG.
- Each benchmark runs on one fewer thread than `nproc` reports, and the script ignores any `NUM_THREADS` you set.
- Results are saved as `results/PROGRAM.json` and `results/PROGRAM.txt`, where `PROGRAM` is the name of the benchmark program.
- Set `BENCHMARK_REPS` to change the number of repetitions (default 5), whose median is reported.

### Short test

Run `make short-test` to test every PRNG to 256MB with each seed type (`default`, `pattern`, `random`, and `zero`).
- It runs `make benchmark` first.
- It takes about 4 minutes to finish.
- The PractRand output goes to `results/prng-results.tlmax-256MB.summary.txt`, so that part prints nothing while it runs.

### Long test

Run `make long-test` to test every PRNG to 512GB with random seeds.
- It runs `make short-test` first, so the benchmark and 256MB results are also overwritten.
- It takes about 27.7 hours to finish.
- The PractRand output goes to `results/prng-results.tlmax-512GB.summary.txt`.

### Testing some PRNGs

Run `bash test-prng-dump.bash -m 256MB -s zero NAME...` to test only some PRNGs.
- It overwrites the matching files in the `results` folder.
- Run `bash test-prng-dump.bash -h` for the options.

### Updating results

Run `make update-short-test` or `make update-long-test` to regenerate the result lists and JSON files from existing test outputs.
- No tests are run.
- Newer benchmark data is included.
- The summary goes to a `.summary.dry-run.txt` file, so the summary of the real run is kept.

### Plotting

Results are saved in the `results` folder.
To plot them, run `python3 results/plot-results.py FILE`, where `FILE` is a `results/prng-results.*.json` file.  Add `-o OUT.png` to save the plot to a file instead of showing it.

### Warm-up survey

Run `./warmup-survey` to survey the warm-ups.
- Some PRNGs discard their first outputs in `init()` (a warm-up), because with the zero seed those outputs have mostly zero bits.
- For several PRNGs, the zero seed failed PractRand right away without the warm-up.
- For each PRNG with a warm-up, the survey starts from the state before the warm-up and reports how many outputs to discard before they look filled.
- The counts in `init()` (`num_warmup_discards`) are not read by the survey, so update them by hand after rerunning it.

## Adding a PRNG

To add a PRNG, follow the registration steps under Architecture in [CLAUDE.md](CLAUDE.md).

## Requirements

The following are required to build:
- [g++](https://gcc.gnu.org/) with C++26 support (clang++ is not supported)
- [Google Benchmark](https://github.com/google/benchmark)

The following programs are required to run:
- [datamash](https://www.gnu.org/software/datamash/)
- [jq](https://github.com/jqlang/jq) 1.8.0 or later
- [parallel](https://www.gnu.org/software/parallel/)
- `RNG_test` from [PractRand](https://github.com/planet36/PractRand)

The Makefile checks for these programs before it does anything, so even `make` fails if one is missing.  See the [Makefile](Makefile) for the full list.

The following are required to plot the results:
- [Python](https://www.python.org/) 3.9 or later
- [Matplotlib](https://matplotlib.org/)
