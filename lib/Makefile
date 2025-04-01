blddir ?= build
ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-lib := $(blddir)/$(ub-dir)/libub.a
ub-incs := -I$(ub-dir)/include
ub-configure := python $(ub-dir)/configure.py $(ub-yamls)

all-ub: $(ub-lib)

include $(ub-dir)/algo/Makefile
include $(ub-dir)/broker/Makefile
include $(ub-dir)/logger/Makefile
include $(ub-dir)/osal/Makefile

$(ub-lib): $(ub-objs)
	ar rcs $@ $(ub-objs)

include $(ub-dir)/cflags.mk
$(ub-objs): CC := gcc
$(ub-objs): CFLAGS := -std=gnu11 $(ub-cflags) $(ub-incs)

$(blddir)/%.o: %.c | $(ub-blddirs)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ub-blddirs):
	mkdir -p $@

.PHONY: all-ub
