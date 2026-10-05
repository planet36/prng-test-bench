# prng-test-bench

C++ PRNG test bench that uses [Google Benchmark](https://github.com/google/benchmark) (to measure speed) and [PractRand](https://github.com/planet36/PractRand) (to measure randomness).

## Building

Run `make` to build `prng-dump`, the benchmark programs (`prng-next-benchmark` and `prng-construct-benchmark`), and `warmup-survey`.

`prng-dump` writes the raw output of a PRNG to stdout:
* `./prng-dump -i` lists the available PRNGs
* `./prng-dump -s {default|pattern|random|zero} [-l GiB] NAME` dumps the output of one PRNG
* `./prng-dump -h` prints the full usage

For example, to test one PRNG by hand:
```sh
./prng-dump -s zero wyrand | RNG_test stdin64 -tlmax 256MB -multithreaded
```

The second column of `./prng-dump -i` is the number of uniformly random bits each call returns.  `prng-dump` writes each value in a word of the largest power of 2 bits that is at most that number, and the `stdinN` given to `RNG_test` has to match that word.  For example, `std::ranlux24` returns 24 bits, so it needs `stdin16`.  `RNG_test` accepts at most `stdin64`, so a 128-bit PRNG also uses `stdin64`.

## Usage

To run the benchmarks only: `make benchmark`
* Runs `run-benchmarks.bash`, which runs each `*-benchmark` program
* `prng-next-benchmark` measures how fast each PRNG generates values (GiB/s per thread)
* `prng-construct-benchmark` measures how long it takes to construct a randomly seeded PRNG
* Results are saved as `results/PROGRAM.json` and `results/PROGRAM.txt`
* Set `BENCHMARK_REPS` to change the number of repetitions (default 5); the median is reported

To run the short test: `make short-test`
* Tests to 256MB with each seed type (`default`, `pattern`, `random`, and `zero`)
* `benchmark` is a prerequisite
* Takes about 4 minutes to finish

To run the long test: `make long-test`
* Tests to 512GB with random seeds
* `benchmark` is a prerequisite
* Takes about 23.8 hours to finish

To test only some PRNGs: `bash test-prng-dump.bash -m 256MB -s zero NAME...`
* Overwrites the matching files in the `results` folder
* Run `bash test-prng-dump.bash -h` for the options

To regenerate the summary files from existing test outputs: `make update-short-test` or `make update-long-test`
* No tests are run
* Newer benchmark data is included

Results are saved in the `results` folder.
To plot them, run `python3 results/plot-results.py FILE`, where `FILE` is a `results/prng-results.*.json` file.

To survey the warm-ups, run `./warmup-survey`.
* Some PRNGs discard their first outputs in `init()` (a warm-up), because with the zero seed those outputs have mostly zero bits
* For several PRNGs, the zero seed failed PractRand right away without the warm-up
* For each PRNG with a warm-up, the survey starts from the state before the warm-up and reports how many outputs to discard before they look filled
* The counts in `init()` (`num_warmup_discards`) are not read by the survey, so update them by hand after rerunning it

## Requirements

The following are required to build:
- [g++](https://gcc.gnu.org/) with C++26 support (clang++ is not supported)
- [Google Benchmark](https://github.com/google/benchmark)

The following programs are required to run:
- [datamash](https://www.gnu.org/software/datamash/)
- [jq](https://github.com/jqlang/jq) 1.8.0 or later
- [parallel](https://www.gnu.org/software/parallel/)
- `RNG_test` from [PractRand](https://github.com/planet36/PractRand)
- See [Makefile](Makefile) for detailed list

The following are required to plot the results:
- [Python 3](https://www.python.org/)
- [Matplotlib](https://matplotlib.org/)
