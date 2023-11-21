# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

# XXX: I wish jq had "tobool"
# https://github.com/jqlang/jq/pull/2098
# https://stackoverflow.com/a/42706666
# https://stackoverflow.com/a/76733482
def tobool:
  if . | ascii_downcase == "true" then true
  else false
  end;
