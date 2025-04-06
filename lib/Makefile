blddir ?= build
ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-lib := $(blddir)/$(ub-dir)/libub.a
ub-incs := -I$(ub-dir)/include
ub-getcfg := python $(ub-dir)/getcfg.py $(ub-yamls) $(ub-dir)/default.yaml

all-ub: $(ub-lib)

include $(ub-dir)/algo/Makefile
include $(ub-dir)/broker/Makefile
include $(ub-dir)/logger/Makefile
include $(ub-dir)/osal/Makefile
include $(ub-dir)/cver.mk
include $(ub-dir)/cflags.mk

$(ub-objs): CC := gcc
$(ub-objs): CFLAGS := $(ub-cver) $(ub-cflags) $(ub-incs)

$(ub-lib): $(ub-objs)
	ar rcs $@ $(ub-objs)

$(blddir)/$(ub-dir)/%.o: $(ub-dir)/%.c | $(ub-blddirs)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ub-blddirs):
	mkdir -p $@

.PHONY: all-ub
