blddir ?= build
ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-blddir := $(patsubst %/.,%,$(blddir)/$(ub-dir))
ub-lib := $(ub-blddir)/libub.a
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

$(ub-blddir)/%.o: $(ub-dir)/%.c | $(ub-blddirs)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ub-blddirs):
	mkdir -p $@

.PHONY: all-ub
