CC := gcc
sample_DIR := sample

usage:
	@echo Usage:
	@echo "make run-*";
	@echo "make coverage-*";
	@echo "rm -rf build-*".

include src/Makefile
include test/Makefile

.PHONY: usage
