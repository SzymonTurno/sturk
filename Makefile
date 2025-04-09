usage:
	@echo Usage: make ub-yamls="path/to/some.yaml" run-*

include lib/Makefile
include examples/ex1/Makefile
include tests/Makefile

.PHONY: usage
