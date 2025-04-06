blddir ?= build
ex2-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex2-target := $(blddir)/$(ex2-dir)/app
ex2-objs := $(blddir)/$(ex2-dir)/main.o

run-ex2: $(ex2-target)
	./$(ex2-target)

$(ex2-target): $(blddir)/$(ex2-dir) $(ex2-objs)
	make all-ub
	gcc -o $@ $(ex2-objs) $(ub-lib)

$(ex2-objs): CC = gcc
$(ex2-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)
$(ex2-objs): CFLAGS += -DEXPECTED_OUT="\"$(ex2-dir)/expected.out\""

$(blddir)/$(ex2-dir):
	mkdir -p $@

.PHONY: run-ex2
