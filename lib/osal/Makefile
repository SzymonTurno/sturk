osal_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
osal_blddir := $(blddir)/$(osal_dir)
ub-blddirs += $(osal_blddir)/none
ub-blddirs += $(osal_blddir)/posix

getcfg := $(ub-getcfg) $(osal_dir)/default.yaml -s osal
ub-objs += $(osal_blddir)/$(shell $(getcfg) -p mem)/mem.o
ub-objs += $(osal_blddir)/$(shell $(getcfg) -p mutex)/mutex.o
ub-objs += $(osal_blddir)/$(shell $(getcfg) -p sem)/sem.o
ub-objs += $(osal_blddir)/$(shell $(getcfg) -p sysio)/sysio.o
