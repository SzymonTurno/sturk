MAKE := make
ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif
PYTHON := python -B

all: cantil-default

check: check-default

configure:
	$(PYTHON) ./tools/olgite.py ./olconf.py | grep "_OLCONF" > mk/olconf.mk

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

docs:
	doxygen Doxyfile

include mk/olconf.mk
include mk/default/phony.mk
include mk/iso/phony.mk
include mk/posix/phony.mk

.PHONY: all check format configure docs
