cantil-iso: build-iso/src/libcantil.a
build-iso/src/libcantil.a: build-iso/Makefile
ifeq ($(OS),Windows_NT)
	cd build-iso && $(MAKE) src\\\libcantil.a
else
	cd build-iso && $(MAKE) src/libcantil.a
endif

unity-iso: build-iso/Unity
build-iso/Unity: build-iso/Makefile
	cd build-iso && $(MAKE) Unity

all-iso: build-iso/tests/app
build-iso/tests/app: build-iso/Makefile
ifeq ($(OS),Windows_NT)
	cd build-iso && $(MAKE) tests\\\app
else
	cd build-iso && $(MAKE) tests/app
endif

check-iso: build-iso/tests/reports.d/valgrind.info
build-iso/tests/reports.d/valgrind.info: build-iso/Makefile
ifeq ($(OS),Windows_NT)
	cd build-iso && $(MAKE) tests\\\reports.d\\\valgrind.info
else
	cd build-iso && $(MAKE) tests/reports.d/valgrind.info
endif

coverage-iso: build-iso/tests/reports.d/coverage.d
build-iso/tests/reports.d/coverage.d: build-iso/Makefile
	cd build-iso && $(MAKE) tests/reports.d/coverage.d

build-iso/Makefile: build-iso
	cd build-iso && $(mkfile_DIR)/tools/configure.sh \
		$(mkfile_DIR)/configs/iso/olconf.yaml

build-iso:
	$(call MKDIR, build-iso)

.PHONY: cantil-iso unity-iso all-iso check-iso coverage-iso
