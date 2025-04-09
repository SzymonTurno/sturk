blddir ?= build
ex1-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex1-blddir := $(blddir)/$(ex1-dir)
ex1-objs := $(ex1-blddir)/main.o $(ex1-blddir)/middleware.o

run-ex1: $(ex1-blddir)/app
	$(ub-runcmd) ./$(ex1-blddir)/app

$(ex1-objs): CC := gcc
$(ex1-objs): CFLAGS := $(ub-cflags) -DEXPECTED_OUT="\"$(ex1-dir)/expected.out\""

$(ex1-blddir)/app: $(ex1-objs) $(ub-lib)
	gcc -o $@ $(ex1-objs) $(ub-lib)

$(ex1-blddir)/%.o: $(ex1-dir)/%.c | $(ex1-blddir)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ex1-blddir):
	mkdir -p $@

.PHONY: run-ex1
