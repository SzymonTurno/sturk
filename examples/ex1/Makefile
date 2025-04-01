blddir ?= build
ex1-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex1-target := $(blddir)/$(ex1-dir)/app
ex1-objs := $(blddir)/$(ex1-dir)/main.o $(blddir)/$(ex1-dir)/middleware.o

run-ex1: $(ex1-target)
	./$(ex1-target)

$(ex1-target): $(blddir)/$(ex1-dir) $(ex1-objs)
	make all-ub ub-yamls=$(ex1-dir)/ub.yaml
	gcc -o $@ $(ex1-objs) $(ub-lib)

$(ex1-objs): CC = gcc
$(ex1-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)
$(ex1-objs): CFLAGS += -DEXPECTED_OUT="\"$(ex1-dir)/expected.out\""

$(blddir)/$(ex1-dir):
	mkdir -p $@

.PHONY: run-ex1
