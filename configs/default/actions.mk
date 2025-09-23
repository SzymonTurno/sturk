sturk-default: build/src/libsturk.a
build/src/libsturk.a: build/Makefile
ifeq ($(OS),Windows_NT)
	cd build && $(MAKE) src\\\libsturk.a
else
	cd build && $(MAKE) src/libsturk.a
endif

unity-default: build/Unity
build/Unity: build/Makefile
	cd build && $(MAKE) Unity

all-default: build/tests/app
build/tests/app: build/Makefile
ifeq ($(OS),Windows_NT)
	cd build && $(MAKE) tests\\\app
else
	cd build && $(MAKE) tests/app
endif

check-default: build/tests/reports.d/valgrind.info
build/tests/reports.d/valgrind.info: build/Makefile
ifeq ($(OS),Windows_NT)
	cd build && $(MAKE) tests\\\reports.d\\\valgrind.info
else
	cd build && $(MAKE) tests/reports.d/valgrind.info
endif

build/Makefile: build
	cd build && $(mkfile_DIR)/tools/configure.sh

build:
	$(call MKDIR, $@)

.PHONY: sturk-default unity-default all-default check-default
