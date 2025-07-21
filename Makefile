MAKE := make
ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif
PYTHON := python -B

ifneq ($(lastword $(MAKEFILE_LIST)),Makefile)

	MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
	CURRENT_DIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))

all:
	make -f $(CURRENT_DIR)/mk/Makefile \
		$(shell $(PYTHON) $(CURRENT_DIR)/tools/olgite.py \
			$(CURRENT_DIR)/olconf.py $(cantil_YAMLS))

.PHONY: all

else

all: cantil-default

check: check-default

configure:
	$(PYTHON) ./tools/olgite.py ./olconf.py | grep "_OLCONF" > mk/olconf.mk

format:
	find . -iname '*.h' -o -iname '*.c' | xargs clang-format -i

Unity:
	$(MAKE) -f ./mk/Makefile Unity

include mk/olconf.mk
include mk/default/phony.mk
include mk/iso/phony.mk
include mk/posix/phony.mk

.PHONY: all check configure format

endif
