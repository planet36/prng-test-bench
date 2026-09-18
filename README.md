# prng-test-bench

C++ PRNG test bench that uses [hyperfine](https://github.com/sharkdp/hyperfine) (to measure speed) and [PractRand](https://github.com/planet36/PractRand) (to measure randomness).

## Building

Run `make` to build `prng-dump`.

`prng-dump` writes the raw output of a PRNG to stdout:
* `./prng-dump -i` lists the available PRNGs
* `./prng-dump -s {default|pattern|random|zero} [-l GiB] NAME` dumps the output of one PRNG
* `./prng-dump -h` prints the full usage

For example, to test one PRNG by hand:
```sh
./prng-dump -s zero wyrand | RNG_test stdin64 -tlmax 256MB -multithreaded
```

## Usage

To run the benchmark only: `make prng-bench`
* Takes about 9 minutes to finish

To run the short test: `make short-test`
* Tests to 256MB with each seed type (`default`, `pattern`, `random`, and `zero`)
* `prng-bench` is a prerequisite
* Takes about 11 minutes to finish

To run the long test: `make long-test`
* Tests to 512GB with random seeds
* `prng-bench` is a prerequisite
* Takes about 40 hours to finish

To test only some PRNGs: `bash test-prng-dump.bash -m 256MB -s zero NAME...`
* Overwrites the matching files in the `results` folder
* Run `bash test-prng-dump.bash -h` for the options

To regenerate the summary files from existing test outputs: `make update-short-test` or `make update-long-test`
* No tests are run
* Newer benchmark data is included

Results are saved in the `results` folder.
To plot them, run `python3 results/plot-results.py FILE`, where `FILE` is a `results/prng-results.*.json` file.

To survey the warm-ups, build and run `warmup-survey.cpp` (`make` does not build it):
```sh
g++ -std=c++26 -O2 -march=native -I include warmup-survey.cpp -o warmup-survey && ./warmup-survey
```
* Some PRNGs discard their first outputs in `init()` (a warm-up), because without it they fail PractRand right away with the zero seed
* For each of them, the survey starts from the state before the warm-up and reports how many outputs to discard before they look filled
* The counts in `init()` (`num_warmup_discards`) are not read by the survey, so update them by hand after rerunning it

## Requirements

The following compiler is required to build:
- [g++](https://gcc.gnu.org/) with C++26 support (clang++ is not supported)

The following programs are required to run:
- [datamash](https://www.gnu.org/software/datamash/)
- [hyperfine](https://github.com/sharkdp/hyperfine)
- [jq](https://github.com/jqlang/jq) 1.8.0 or later
- [parallel](https://www.gnu.org/software/parallel/)
- `RNG_test` from [PractRand](https://github.com/planet36/PractRand)
- See [Makefile](Makefile) for detailed list

The following are required to plot the results:
- [Python](https://www.python.org/) 3
- [Matplotlib](https://matplotlib.org/)
