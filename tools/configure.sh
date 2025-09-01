#!/usr/bin/bash

set -e

TOOLS_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)
STURK_DIR=${TOOLS_DIR%/tools}

python -B $TOOLS_DIR/olgite.py $STURK_DIR/olconf.py $1 > Makefile
