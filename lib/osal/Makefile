ub-osal-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-blddirs += $(blddir)/$(ub-osal-dir)/none
ub-blddirs += $(blddir)/$(ub-osal-dir)/posix

ub-osal-objs += $(shell $(ub-getcfg) $(ub-osal-dir)/default.yaml -s osal)
ub-objs += $(addprefix $(blddir)/$(ub-osal-dir)/,$(ub-osal-objs))
