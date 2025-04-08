# prng-test-bench

C++ PRNG test bench that uses [Google Benchmark](https://github.com/google/benchmark) (to measure speed) and [PractRand](https://github.com/planet36/PractRand) (to measure randomness)

## Usage

To run the benchmark only: `make bench`

To run the short test: `make short-test`
* `benchmark` is a prerequisite
* Takes about 12.5 minutes to finish

To run the long test: `make long-test`
* `benchmark` is a prerequisite
* Takes about 40 hours to finish

Results are saved in the "results" folder.

## Requirements

The following libraries are required to build:
- [fmt](https://github.com/fmtlib/fmt)

The following programs are required to run:
- [hyperfine](https://github.com/sharkdp/hyperfine)
- [jq](https://github.com/jqlang/jq)
- [parallel](https://www.gnu.org/software/parallel/)
- [PractRand](https://github.com/planet36/PractRand)
- See [Makefile](Makefile) for detailed list
