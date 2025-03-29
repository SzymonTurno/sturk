all: run-example

include lib/Makefile
include example/Makefile

clean: clean-example clean-ub

.PHONY: all clean
