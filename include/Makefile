# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

export LC_ALL := C

# clang++ not supported
CXX = g++

CPPFLAGS = -MMD -MP

# gnu++ needed for static_assert(std::integral<__int128_t>);
CXXFLAGS = -std=gnu++26
CXXFLAGS += -pipe -Wall -Wextra -Wpedantic -Wfatal-errors
CXXFLAGS += -O3 -flto=auto -march=native -fno-math-errno
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Werror -Wno-error=cpp -Wno-error=pedantic

#CXXFLAGS += -march=raptorlake

#LDFLAGS +=

#LDLIBS +=

HDRS := $(wildcard *.hpp)
DEPS = $(HDRS:.hpp=.d)
OBJS := $(addsuffix .o, $(basename $(HDRS)))
BINS := $(addsuffix .out, $(basename $(HDRS)))

all: $(OBJS)

bins: $(BINS)

objs: $(OBJS)

%.o: %.hpp
	@# This is done to prevent "warning: #pragma once in main file".
	-printf '#include "%s"\n' $< | LC_ALL=C $(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -o $@ - -c

%.out: %.hpp
	@# This is done to prevent "warning: #pragma once in main file".
	-printf '#include "%s"\nint main(){return 0;}\n' $< | LC_ALL=C $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -x c++ -o $@ - $(LDLIBS)

clean:
	@$(RM) --verbose -- $(DEPS) $(OBJS) $(BINS)

lint:
	-clang-tidy --quiet $(HDRS) -- $(CPPFLAGS) $(CXXFLAGS)

# https://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: all bins objs clean lint

-include $(DEPS)
