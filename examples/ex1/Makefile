ex1-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex1-build := build/$(ex1-dir)
ex1-target := $(ex1-build)/app
ex1-objs := $(ex1-dir)/main.o $(ex1-dir)/middleware.o

run-ex1: $(ex1-target)
	./$(ex1-target)

$(ex1-target): $(ex1-objs) $(ex1-build)
	make all-ub ub-yamls=$(ex1-dir)/ub.yaml
	gcc -o $@ $(ex1-objs) $(ub-lib)
	mv $(ex1-objs) $(ex1-build)/

$(ex1-objs): CC = gcc
$(ex1-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)
$(ex1-objs): CFLAGS += -DEXPECTED_OUT="\"$(ex1-dir)/expected.out\""

$(ex1-build):
	mkdir -p $@

clean-ex1:
	rm -f $(ex1-objs)

.PHONY: run-ex1 clean-ex1
