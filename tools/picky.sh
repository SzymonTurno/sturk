#!/usr/bin/bash

fail_if_stderr() (
  # save current options
  bash_options="${-}"
  
  # disable exit on error
  set +o errexit
  
  # Save return code of command in rc
  rc=$({
    ("$@" 2>&1 >&3 3>&- 4>&-; echo "$?" >&4) |
    grep '^' >&2 3>&- 4>&-
  } 4>&1)
  
  # Save return code of grep in err_in_stderr
  err_in_stderr=$?
  
  # enable exit on error if it was previously enabled
  test "${bash_options#*e*}" != "$bash_options" && set -o errexit
  
  # exit with original return code if it's not zero
  [ "$rc" -eq 0 ] || exit "$rc"
  
  # exit with return code 125 if something was in stderr
  [ "$err_in_stderr" -ne 0 ] || exit 125
) 3>&1

fail_if_stderr $@
