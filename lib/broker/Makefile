ub-broker-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ub-broker-build := build/$(ub-broker-dir)
ub-broker-objs := $(ub-broker-dir)/broker.o

ub-tmp-objs += $(ub-broker-objs)
ub-objs += $(ub-broker-build)/*.o
ub-builds += $(ub-broker-build)

$(ub-broker-build): $(ub-broker-objs)
	mkdir -p $@
	mv $(ub-broker-objs) $(ub-broker-build)/
