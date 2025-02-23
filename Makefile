all: run-example

include src/Makefile
include example/Makefile

clean: clean-example clean-ub

.PHONY: all clean
