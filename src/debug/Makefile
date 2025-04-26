debug_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
debug_blddir := $(blddir)/$(debug_dir)

include $(debug_dir)/cfg.mk

ub-blddirs += $(debug_blddir)
ub-objs += $(debug_blddir)/logger.o
ub-objs += $(debug_blddir)/log.o
ub-objs += $(debug_blddir)/snapshot.o
