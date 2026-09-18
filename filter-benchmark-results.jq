# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: MPL-2.0

.benchmarks[] |
select(.aggregate_name == "median") |
(.name | sub("(/threads:[0-9]+)?_median$"; "")) as $clean_name |
if has("bytes_per_second")
then
    "\($clean_name) \(.bytes_per_second/1E9) GiB/s"
else
    "\($clean_name) \(.cpu_time) \(.time_unit)"
end
