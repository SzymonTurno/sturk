#!/usr/bin/bash

set -e

python3 -m http.server 9999 -d $1
