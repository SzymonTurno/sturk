blddir ?= build
ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-blddir := $(patsubst %/.,%,$(blddir)/$(ub-dir))
ub-lib := $(ub-blddir)/libub.a
ub-incs := -I$(ub-dir)/include
ub-getcfg := python $(ub-dir)/getcfg.py $(ub-yamls) $(ub-dir)/default.yaml

include $(ub-dir)/cfg.mk
include $(ub-dir)/algo/Makefile
include $(ub-dir)/broker/Makefile
include $(ub-dir)/debug/Makefile
include $(ub-dir)/osal/Makefile

all-ub: $(ub-lib)

$(ub-lib): $(ub-objs)
	ar rcs $@ $(ub-objs)

$(ub-objs): CFLAGS ?= $(ub-gcov) $(ub-cflags)
$(ub-blddir)/%.o: $(ub-dir)/%.c | $(ub-blddirs)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ub-blddirs):
	mkdir -p $@

.PHONY: all-ub
