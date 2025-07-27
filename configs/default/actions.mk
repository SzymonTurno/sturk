cantil-default: build/src/libcantil.a
build/src/libcantil.a: build/Makefile
	cd build && $(MAKE) src/libcantil.a

unity-default: build/Unity
build/Unity: build/Makefile
	cd build && $(MAKE) Unity

all-default: build/tests/app
build/tests/app: build/Makefile
	cd build && $(MAKE) tests/app

check-default: build/tests/reports.d/valgrind.info
build/tests/reports.d/valgrind.info: build/Makefile
	cd build && $(MAKE) tests/reports.d/valgrind.info

build/Makefile: build
	cd build && $(mkfile_DIR)/tools/configure.sh

build:
	$(call MKDIR, build)

.PHONY: cantil-default unity-default all-default check-default
