ub-algo-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-algo-build := build/$(ub-algo-dir)
ub-algo-objs := $(ub-algo-dir)/pool.o

ub-tmp-objs += $(ub-algo-objs)
ub-objs += $(ub-algo-build)/*.o
ub-builds += $(ub-algo-build)

$(ub-algo-build): $(ub-algo-objs)
	@echo "[UB] Creating algo."
	mkdir -p $@
	mv $(ub-algo-objs) $(ub-algo-build)/
