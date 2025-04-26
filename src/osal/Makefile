osal_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
osal_blddir := $(blddir)/$(osal_dir)
osal_getcfg := $(ub-getcfg) $(osal_dir)/default.yaml -s osal

ub-blddirs += $(osal_blddir)/none
ub-blddirs += $(osal_blddir)/posix
ub-objs += $(osal_blddir)/$(shell $(osal_getcfg) -p mem)/mem.o
ub-objs += $(osal_blddir)/$(shell $(osal_getcfg) -p mutex)/mutex.o
ub-objs += $(osal_blddir)/$(shell $(osal_getcfg) -p sem)/sem.o
ub-objs += $(osal_blddir)/$(shell $(osal_getcfg) -p sysio)/sysio.o
