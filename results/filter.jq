# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

import "tobool" as tobool;
split(" ") |
{
prng_name: .[0],
test_ended_at_exp2_bytes: .[1] | tonumber,
test_ended_with_failure: .[2] | tobool::tobool,
prng_throughput_mebibytes: .[3] | tonumber,
}
