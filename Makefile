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

public:
ifeq ($(PROJECT_NUMBER),)
	(cat docs/Doxyfile ; \
	echo "PROJECT_NUMBER=$(shell git describe --tags --abbrev=0 \
		| sed 's/v//')") | doxygen -
else
	(cat docs/Doxyfile ; echo "PROJECT_NUMBER=$(PROJECT_NUMBER)") | doxygen -
endif

include configs/default/actions.mk
include configs/iso/actions.mk
include configs/posix/actions.mk

.PHONY: all check format
