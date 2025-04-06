blddir ?= build
ex1-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex1a-target := $(blddir)/$(ex1-dir)/app_a
ex1b-target := $(blddir)/$(ex1-dir)/app_b
ex1-objs := $(blddir)/$(ex1-dir)/main.o $(blddir)/$(ex1-dir)/middleware.o

run-ex1a: $(ex1a-target)
	./$(ex1a-target)

run-ex1b: $(ex1b-target)
	./$(ex1b-target)

$(ex1-objs): CC = gcc
$(ex1-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)
$(ex1-objs): CFLAGS += -DEXPECTED_OUT="\"$(ex1-dir)/expected.out\""

$(ex1a-target): $(blddir)/$(ex1-dir) $(ex1-objs)
	make all-ub ub-yamls=$(ex1-dir)/posix.yaml
	gcc -o $@ $(ex1-objs) $(ub-lib)

$(ex1b-target): $(blddir)/$(ex1-dir) $(ex1-objs)
	make all-ub ub-yamls=$(ex1-dir)/iso.yaml
	gcc -o $@ $(ex1-objs) $(ub-lib)

$(blddir)/$(ex1-dir)/%.o: $(ex1-dir)/%.c | $(blddir)/$(ex1-dir)
	$(CC) $(CFLAGS) -c -o $@ $<

$(blddir)/$(ex1-dir):
	mkdir -p $@

.PHONY: run-ex1a run-ex1b
