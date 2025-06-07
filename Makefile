CC := gcc
sample_DIR := sample

usage:
	@echo Usage:
	@echo "1. \`make run-*\`,"
	@echo "2. \`make coverage-*\`,"
	@echo "3. \`cat build-*/src/analysis.d/valgrind.info\`,"
	@echo "4. \`rm -rf build-*\`."

include src/Makefile
include test/Makefile

.PHONY: usage
