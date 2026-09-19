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
* Takes about 5 minutes to finish

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
* Some PRNGs discard their first outputs in `init()` (a warm-up), because without it they fail PractRand right away with the zero seed
* For each of them, the survey starts from the state before the warm-up and reports how many outputs to discard before they look filled
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
- [Python](https://www.python.org/) 3
- [Matplotlib](https://matplotlib.org/)
