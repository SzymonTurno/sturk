ub-osal-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-osal-build := build/$(ub-osal-dir)
ub-osal-objs := $(shell $(ub-configure) $(ub-osal-dir)/default.yaml -f osal)

ub-tmp-objs += $(ub-osal-objs)
ub-objs += $(ub-osal-build)/*.o
ub-builds += $(ub-osal-build)

$(ub-osal-build): $(ub-osal-objs)
	@echo "[UB] Creating osal."
	mkdir -p $@
	mv $(ub-osal-objs) $(ub-osal-build)/
