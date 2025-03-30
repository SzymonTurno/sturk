ub-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

ub-build := build/$(ub-dir)
ub-lib := $(ub-build)/libub.a
ub-incs := -I$(ub-dir)/include

all-ub: $(ub-lib)

include $(ub-dir)/algo/Makefile
include $(ub-dir)/logger/Makefile
include $(ub-dir)/MoM/Makefile
include $(ub-dir)/osal/Makefile

$(ub-lib): $(ub-builds)
	ar rcs $@ $(ub-objs)

clean-ub:
	rm -f $(ub-tmp-objs)

include $(ub-dir)/cflags.mk
$(ub-tmp-objs): CC := gcc
$(ub-tmp-objs): CFLAGS := -std=gnu11 $(ub-cflags) $(ub-incs)

.PHONY: all-ub clean-ub
