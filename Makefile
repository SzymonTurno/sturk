CC := gcc
blddir := build
rptdir := $(blddir)/analysis.d

usage:
	@echo Usage: make run-* ub-yamls="path/to/optional.yaml"

coverage: $(rptdir)/out

$(rptdir)/out: $(rptdir)/coverage.info
	genhtml $(rptdir)/coverage.info --output-directory $@

$(rptdir)/coverage.info: $(rptdir)
	lcov --capture --directory ./$(blddir) --output-file $@

$(rptdir): $(blddir)
	mkdir -p $@

include lib/Makefile
include examples/ex1/Makefile
include tests/Makefile

.PHONY: usage coverage
