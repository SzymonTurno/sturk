MAKE := make
ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif
PYTHON := python -B

ifneq ($(lastword $(MAKEFILE_LIST)),Makefile)

	MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
	MKFILE_DIR := $(patsubst %/,%,$(dir $(MKFILE_PATH)))

all:
	make -f $(MKFILE_DIR)/mk/Makefile \
		$(shell $(PYTHON) $(MKFILE_DIR)/tools/olgite.py \
			$(MKFILE_DIR)/olconf.py $(cantil_YAMLS))

.PHONY: all

else

all: cantil-default

check: check-default

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

Unity:
	$(MAKE) -f ./mk/Makefile Unity

include mk/default/phony.mk
include mk/iso/phony.mk
include mk/posix/phony.mk

.PHONY: all check format

endif
