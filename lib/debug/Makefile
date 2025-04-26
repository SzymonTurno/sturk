include cfg.mk

debug_blddir := $(blddir)/$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

ub-blddirs += $(debug_blddir)
ub-objs += $(debug_blddir)/logger.o
ub-objs += $(debug_blddir)/log.o
ub-objs += $(debug_blddir)/snapshot.o
