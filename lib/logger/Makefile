ub-logger-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-logger-build := build/$(ub-logger-dir)
ub-logger-objs := $(ub-logger-dir)/logger.o

ub-tmp-objs += $(ub-logger-objs)
ub-objs += $(ub-logger-build)/*.o
ub-builds += $(ub-logger-build)

$(ub-logger-build): $(ub-logger-objs)
	mkdir -p $@
	mv $(ub-logger-objs) $(ub-logger-build)/
