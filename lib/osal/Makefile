ub-osal-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-osal-blddir := $(blddir)/$(ub-osal-dir)
ub-blddirs += $(ub-osal-blddir)/none
ub-blddirs += $(ub-osal-blddir)/posix

getcfg := $(ub-getcfg) $(ub-osal-dir)/default.yaml -s osal
ub-objs += $(ub-osal-blddir)/$(shell $(getcfg) -p mem)/mem.o
ub-objs += $(ub-osal-blddir)/$(shell $(getcfg) -p mutex)/mutex.o
ub-objs += $(ub-osal-blddir)/$(shell $(getcfg) -p sem)/sem.o
ub-objs += $(ub-osal-blddir)/$(shell $(getcfg) -p sysio)/sysio.o
