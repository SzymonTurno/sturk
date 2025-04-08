usage:
	@echo Usage: make run-*

valgrind-run := valgrind valgrind --leak-check=full --show-leak-kinds=all
valgrind-run += --track-origins=yes --log-file="tmp.log"

include lib/Makefile
include examples/ex1/Makefile
include tests/Makefile

build/report.log:
ifneq ($(wildcard build/.*),)
	$(error Remove build directory)
endif
	make $(ex1a-target)
	$(valgrind-run) $(ex1a-target)
	mv tmp.log report.log
	rm -rf build/
	make $(ex1b-target)
	$(valgrind-run) $(ex1b-target)
	cat tmp.log >> report.log
	rm -rf build/
	make $(ut-gnu-target)
	$(valgrind-run) $(ut-gnu-target) >> report.log
	cat tmp.log >> report.log
	rm -rf build/
	make $(ut-iso-target)
	$(valgrind-run) $(ut-iso-target) >> report.log
	cat tmp.log >> report.log
	rm tmp.log
	rm -rf build/
	mkdir build
	mv report.log build/

.PHONY: usage
