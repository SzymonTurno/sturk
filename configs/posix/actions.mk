cantil-posix: build-posix/src/libcantil.a
build-posix/src/libcantil.a: build-posix/Makefile
	cd build-posix && $(MAKE) src/libcantil.a

unity-posix: build-posix/Unity
build-posix/Unity: build-posix/Makefile
	cd build-posix && $(MAKE) Unity

all-posix: build-posix/tests/app
build-posix/tests/app: build-posix/Makefile
	cd build-posix && $(MAKE) tests/app

check-posix: build-posix/tests/reports.d/valgrind.info
build-posix/tests/reports.d/valgrind.info: build-posix/Makefile
	cd build-posix && $(MAKE) tests/reports.d/valgrind.info

coverage-posix: build-posix/tests/reports.d/coverage.d
build-posix/tests/reports.d/coverage.d: build-posix/Makefile
	cd build-posix && $(MAKE) tests/reports.d/coverage.d

build-posix/Makefile: build-posix
	cd build-posix && $(mkfile_DIR)/tools/configure.sh \
		$(mkfile_DIR)/configs/posix/olconf.yaml

build-posix:
	$(call MKDIR, build-posix)

.PHONY: cantil-posix unity-posix all-posix check-posix coverage-posix
