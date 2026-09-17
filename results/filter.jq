# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

split(" ") |
if length != 4 then error("expected 4 fields: \(join(" "))") end |
{
  prng_name: .[0],
  test_ended_at_exp2_bytes: .[1] | tonumber,
  test_ended_with_failure: .[2] | toboolean,
  prng_throughput_gibibytes_per_second: .[3] | tonumber,
}
