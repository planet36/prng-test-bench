# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: MPL-2.0

export LC_ALL = C

# https://how.wtf/check-if-a-program-exists-from-a-makefile.html
REQUIRED_BINS := \
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
readelf \
rm \
RNG_test \
sed \
sort \
tail \
time \
tr

$(foreach bin,$(REQUIRED_BINS),\
    $(if $(shell command -v $(bin) 2> /dev/null),,$(error Please install `$(bin)`)))

# clang++ not supported
CXX = g++

CPPFLAGS = -MMD -MP
CPPFLAGS += -I include

CXXFLAGS = -std=c++26
CXXFLAGS += -pipe -Wall -Wextra -Wpedantic -Wfatal-errors
CXXFLAGS += -O3 -flto=auto
CXXFLAGS += -Wno-unused-function
# -frecord-gcc-switches is used by readelf
CXXFLAGS += -frecord-gcc-switches

CXXFLAGS += -march=native
#CXXFLAGS += -march=x86-64-v3 -maes -mpclmul -mvaes
#CXXFLAGS += -march=raptorlake

#LDFLAGS =

LDLIBS = -lbenchmark

OUTPUT_DIR = results

SRCS = $(wildcard *.cpp)
DEPS = $(SRCS:.cpp=.d)
BINS = $(basename $(SRCS))

J_SHORT = 50%
TF_SHORT = 1
TLMAX_SHORT = 256MB

J_LONG = 1
TF_LONG = 2
TLMAX_LONG = 512GB

all: $(BINS)

# The built-in recipe for the implicit rule uses $^ instead of $<
%: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)
	@# Extract compile options
	@if [ "$@" = "prng-dump" ]; then \
		readelf -p .GCC.command.line "$@" | grep -F 'GNU GIMPLE' | \
			sed -E -e 's/^\s*\[\s*[0-9]+\]\s*//' | tr -d '\n' > "$@".opts ; \
	fi

benchmark: $(BINS) | $(OUTPUT_DIR)
	bash run-benchmarks.bash

# Takes about 10 mins
short-test: prng-dump benchmark | $(OUTPUT_DIR)
	bash test-prng-dump.bash -j $(J_SHORT) -f $(TF_SHORT) -m $(TLMAX_SHORT) \
		-s default -s pattern -s random -s zero \
		&> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_SHORT).summary.txt

# Takes about 23.8 hrs (random)
long-test: prng-dump benchmark | $(OUTPUT_DIR)
	bash test-prng-dump.bash -j $(J_LONG) -f $(TF_LONG) -m $(TLMAX_LONG) \
		-s random \
		&> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_LONG).summary.txt

# These "update" targets are identical to their respective non-update targets,
# except they pass the dry-run option to the shell script.
# Their purpose is to update the prng-results files with newer benchmark data
# without running the lengthy tests.
# The "benchmark" target should have already been run, but it's not an explicit
# pre-requisite.

update-short-test: prng-dump | $(OUTPUT_DIR)
	bash test-prng-dump.bash -d -j $(J_SHORT) -f $(TF_SHORT) -m $(TLMAX_SHORT) \
		-s default -s pattern -s random -s zero \
		&> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_SHORT).summary.dry-run.txt

update-long-test: prng-dump | $(OUTPUT_DIR)
	bash test-prng-dump.bash -d -j $(J_LONG) -f $(TF_LONG) -m $(TLMAX_LONG) \
		-s random \
		&> $(OUTPUT_DIR)/prng-results.tlmax-$(TLMAX_LONG).summary.dry-run.txt

$(OUTPUT_DIR):
	mkdir --verbose --parents -- $@

clean:
	@$(RM) --verbose -- $(DEPS) $(BINS) *.opts

lint:
	-clang-tidy --quiet $(SRCS) -- $(CPPFLAGS) $(CXXFLAGS)

# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: all benchmark short-test long-test update-short-test update-long-test clean lint

# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html#index-removing-targets-on-failure
.DELETE_ON_ERROR:

-include $(DEPS)
