blddir ?= build
ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-blddir := $(patsubst %/.,%,$(blddir)/$(ub-dir))
ub-lib := $(ub-blddir)/libub.a
ub-incs := -I$(ub-dir)/include
ub-getcfg := python $(ub-dir)/getcfg.py $(ub-yamls) $(ub-dir)/default.yaml

all-ub: $(ub-lib)

include cfg.mk
include algo/Makefile
include broker/Makefile
include debug/Makefile
include osal/Makefile

$(ub-lib): $(ub-objs)
	ar rcs $@ $(ub-objs)

$(ub-objs): CFLAGS ?= $(ub-gcov) $(ub-cflags)
$(ub-blddir)/%.o: $(ub-dir)/%.c | $(ub-blddirs)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ub-blddirs):
	mkdir -p $@

.PHONY: all-ub
