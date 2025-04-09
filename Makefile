usage:
	@echo Usage: make run-* ub-yamls="path/to/optional.yaml"

include lib/Makefile
include examples/ex1/Makefile
include tests/Makefile

.PHONY: usage
