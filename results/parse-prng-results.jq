# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: MPL-2.0

split(" ") |
if length != 5 then error("expected 5 fields: \(join(" "))") end |
if .[4] != "GiB/s" then error("expected unit GiB/s: \(join(" "))") end |
{
  prng_name: .[0],
  test_ended_at_exp2_bytes: .[1] | tonumber,
  test_ended_with_failure: .[2] | toboolean,
  prng_throughput_gibibytes_per_second: .[3] | tonumber,
}
