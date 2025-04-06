blddir ?= build
ex3-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex3-target := $(blddir)/$(ex3-dir)/app
ex3-objs := $(blddir)/$(ex3-dir)/main.o

run-ex3: $(ex3-target)
	./$(ex3-target)

$(ex3-target): $(blddir)/$(ex3-dir) $(ex3-objs)
	make all-ub
	gcc -o $@ $(ex3-objs) $(ub-lib)

$(ex3-objs): CC = gcc
$(ex3-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)
$(ex3-objs): CFLAGS += -DEXPECTED_OUT="\"$(ex3-dir)/expected.out\""

$(blddir)/$(ex3-dir):
	mkdir -p $@

.PHONY: run-ex3
