MAKE:=make
MKDIR:=mkdir -p
PYTHON:=python -B

all:
	$(MAKE) all-default

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

configure:
	$(PYTHON) ./tools/olgite.py ./olconf.py | grep "_OLCONF" > mk/olconf.mk

include mk/olconf.mk
include mk/default/rules.mk
include mk/iso/rules.mk
include mk/posix/rules.mk

.PHONY: all format configure
