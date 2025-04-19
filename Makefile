CC := gcc
blddir := build
rptdir := $(blddir)/analysis.d

usage:
	@echo Usage: make run-* ub-yamls="path/to/optional.yaml"

coverage: $(rptdir)/coverage.d

$(rptdir)/coverage.d: $(rptdir)/coverage.info
	genhtml $(rptdir)/coverage.info --show-details --output-directory $@

$(rptdir)/coverage.info: $(rptdir)
	lcov --capture --directory ./$(blddir) --output-file $@

include lib/Makefile
include tests/Makefile

.PHONY: usage coverage
