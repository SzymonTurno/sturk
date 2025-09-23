ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif
MAKE := make
mkfile_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

all: sturk-default

check: check-default

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

include configs/default/actions.mk
include configs/iso/actions.mk
include configs/posix/actions.mk
include docs/actions.mk

.PHONY: all check format
