blddir ?= build
tests-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
tests-blddir := $(blddir)/$(tests-dir)
tests-objs := $(tests-blddir)/test_algo.o
unity-dir := Unity

run-tests:
	make _run-tests ub-yamls="$(tests-dir)/tests.yaml $(ub-yamls)"
	$(ub-runcmd) ./$(tests-blddir)/app

_run-tests: $(tests-blddir)/app
	$(ub-runcmd) ./$(tests-blddir)/app

$(tests-blddir)/app: $(tests-objs) $(tests-blddir)/unity.o $(ub-lib)
	$(CC) -o $@ $(tests-objs) $(tests-blddir)/unity.o $(ub-lib) $(ub-ldflags)

$(tests-blddir)/%.o: $(tests-dir)/%.c | $(tests-blddir)
	$(CC) $(ub-cflags) -I$(unity-dir)/src -c -o $@ $<

$(tests-blddir)/unity.o: $(unity-dir)/src/unity.c | $(tests-blddir)
	cd $(unity-dir) && git checkout v2.6.1
	$(CC) -I$(unity-dir)/src -c -o $@ $<

$(unity-dir)/src/unity.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(tests-blddir):
	mkdir -p $@

.PHONY: run-tests _run-tests
