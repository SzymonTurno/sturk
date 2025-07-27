cantil-iso: build-iso/src/libcantil.a
build-iso/src/libcantil.a: build-iso/Makefile
	cd build-iso && $(MAKE) src/libcantil.a

unity-iso: build-iso/Unity
build-iso/Unity: build-iso/Makefile
	cd build-iso && $(MAKE) Unity

all-iso: build-iso/tests/app
build-iso/tests/app: build-iso/Makefile
	cd build-iso && $(MAKE) tests/app

check-iso: build-iso/tests/reports.d/valgrind.info
build-iso/tests/reports.d/valgrind.info: build-iso/Makefile
	cd build-iso && $(MAKE) tests/reports.d/valgrind.info

coverage-iso: build-iso/tests/reports.d/coverage.d
build-iso/tests/reports.d/coverage.d: build-iso/Makefile
	cd build-iso && $(MAKE) tests/reports.d/coverage.d

build-iso/Makefile: build-iso
	cd build-iso && $(mkfile_DIR)/tools/configure.sh \
		$(mkfile_DIR)/configs/iso/olconf.yaml

build-iso:
	$(call MKDIR, build-iso)

.PHONY: cantil-iso unity-iso all-iso check-iso coverage-iso
