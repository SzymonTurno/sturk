usage:
	@echo Usage: make run-ex*

include lib/Makefile
include examples/ex1/Makefile
include examples/ex2/Makefile
include examples/ex3/Makefile

.PHONY: usage
