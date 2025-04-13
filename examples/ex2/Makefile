blddir ?= build
ex2-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ex2-blddir := $(blddir)/$(ex2-dir)
ex2-objs := $(ex2-blddir)/main.o $(ex2-blddir)/middleware.o

run-ex2:
	make _run-ex2 ub-yamls="$(ex2-dir)/ex2.yaml $(ub-yamls)"

_run-ex2: $(ex2-blddir)/app
	$(ub-runcmd) ./$(ex2-blddir)/app

$(ex2-blddir)/app: $(ex2-objs) $(ub-lib)
	$(CC) -o $@ $(ex2-objs) $(ub-lib) $(ub-ldflags)

$(ex2-blddir)/%.o: $(ex2-dir)/%.c | $(ex2-blddir)
	$(CC) $(ub-cflags) -c -o $@ $<

$(ex2-blddir):
	mkdir -p $@

.PHONY: run-ex2 _run-ex2
