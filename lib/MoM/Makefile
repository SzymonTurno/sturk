ub-mom-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-mom-build := build/$(ub-mom-dir)
ub-mom-objs := $(ub-mom-dir)/MoM.o

ub-tmp-objs += $(ub-mom-objs)
ub-objs += $(ub-mom-build)/*.o
ub-builds += $(ub-mom-build)

$(ub-mom-build): $(ub-mom-objs)
	@echo "[UB] Creating MoM."
	mkdir -p $@
	mv $(ub-mom-objs) $(ub-mom-build)/
