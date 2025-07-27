#!/usr/bin/bash

fail_if_stderr() (
  rc=$({
    ("$@" 2>&1 >&3 3>&- 4>&-; echo "$?" >&4) |
    grep '^' >&2 3>&- 4>&-
  } 4>&1)
  err=$?
  [ "$rc" -eq 0 ] || exit "$rc"
  [ "$err" -ne 0 ] || exit 125
) 3>&1

fail_if_stderr $@
