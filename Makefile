# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

export LC_ALL := C

# https://how.wtf/check-if-a-program-exists-from-a-makefile.html
REQUIRED_BINS := \
awk \
bash \
cat \
column \
datamash \
diff \
g++ \
grep \
join \
jq \
mkdir \
mktemp \
parallel \
python3 \
readelf \
rm \
RNG_test \
sed \
sort \
sponge \
tail \
time \
tr

$(foreach bin,$(REQUIRED_BINS),\
    $(if $(shell command -v $(bin) 2> /dev/null),,$(error Please install `$(bin)`)))

# clang++ not supported
CXX = g++

CPPFLAGS = -MMD -MP
CPPFLAGS += -Iinclude

CXXFLAGS = -pipe -Wall -Wextra -Wpedantic -Wfatal-errors
CXXFLAGS += -std=c++23
# -frecord-gcc-switches is used by readelf
CXXFLAGS += -frecord-gcc-switches
CXXFLAGS += -O3 -flto=auto -march=native -fno-math-errno

#LDFLAGS +=

LDLIBS += -lbenchmark
LDLIBS += -lfmt

# Should be an odd number for simpler median
NUM_SPEED_TESTS := 11

OUTPUT_DIR := results

SRCS = $(wildcard prng-*.cpp)
DEPS = $(SRCS:.cpp=.d)
BINS = $(basename $(SRCS))

J_SHORT := 50%
TF_SHORT := 0
# For all tf values, PRNGs that fail quickly do so within 256MB.
TLMAX_SHORT := 256MB

J_LONG := 1
# These fail (seed=random) with tf=2 but not tf=1:
#   MRG32k3a, degski64, murmurhash3, splitxix33, tangle
TF_LONG := 2
# With tf=2, PRNGs that pass 512GB also pass 1024GB.
TLMAX_LONG := 512GB

all: $(BINS)

# The built-in recipe for the implicit rule uses $^ instead of $<
# https://www.gnu.org/software/make/manual/html_node/Static-Usage.html
# Static Pattern Rule
$(BINS): prng-% : prng-%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)
	@# Extract compile options
	readelf -p .GCC.command.line $@ | grep -F 'GNU GIMPLE' | sed -E -e 's/^\s*\[\s*[0-9]+\]\s*//' | tr -d '\n' > $@.opts

bench: prng-bench | $(OUTPUT_DIR)
	./$< \
		--benchmark_enable_random_interleaving=true \
		--benchmark_repetitions=$(NUM_SPEED_TESTS) \
		--benchmark_report_aggregates_only=true \
		--benchmark_out_format=json \
		--benchmark_out=$(OUTPUT_DIR)/$<.json

	@# Insert compile options
	jq --rawfile compile_opts $<.opts '. + {compile_opts: $$compile_opts}' < $(OUTPUT_DIR)/$<.json | sponge $(OUTPUT_DIR)/$<.json

	jq -r '.benchmarks[] | select(.aggregate_name == "median") | "\(.run_name)\t\(.bytes_per_second)"' \
		$(OUTPUT_DIR)/$<.json | \
		sed -E -e 's/\/threads:[0-9]+//' | \
		awk '{printf "%s\t%.0f\n", $$1, $$2/(2**20)}' > $(OUTPUT_DIR)/$<.txt

	# (column 2 is MiB/s)
	sort -r -k 2 -g $(OUTPUT_DIR)/$<.txt | column --table

# Takes about 12.5 mins
short-test: prng-dump bench | $(OUTPUT_DIR)
	bash test-prng-dump.bash -j $(J_SHORT) -f $(TF_SHORT) -m $(TLMAX_SHORT) -s default -s pattern -s random -s zero &> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_SHORT).summary.txt

# Takes about 24.2 hrs (random)
# Takes about 44.5 hrs (random, zero)
# Takes about ??.? hrs (zero)
long-test: prng-dump bench | $(OUTPUT_DIR)
	bash test-prng-dump.bash -j $(J_LONG) -f $(TF_LONG) -m $(TLMAX_LONG) -s random -s zero &> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_LONG).summary.txt

$(OUTPUT_DIR):
	mkdir --verbose --parents -- $@

clean:
	@$(RM) --verbose -- $(DEPS) $(BINS) *.opts

lint:
	-clang-tidy --quiet $(SRCS) -- $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS)

# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: all bench short-test long-test clean lint

-include $(DEPS)
