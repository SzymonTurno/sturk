ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif
MAKE := make
mkfile_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
sturk_VERSION := $(shell git describe --tags --abbrev=0 | sed 's/v//')

all: sturk-default

check: check-default

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

public:
	(cat docs/Doxyfile ; echo "PROJECT_NUMBER=$(sturk_VERSION)") | doxygen -

include configs/default/actions.mk
include configs/iso/actions.mk
include configs/posix/actions.mk

.PHONY: all check format
