usage:
	@echo Usage: make run-*

include lib/Makefile
include examples/ex1/Makefile
include tests/Makefile

.PHONY: usage
