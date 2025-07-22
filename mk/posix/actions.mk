cantil-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix build-posix/src/libcantil.a

all-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix build-posix/tests/app

check-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix \
		build-posix/tests/reports.d/valgrind.info

coverage-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix \
		build-posix/tests/reports.d/coverage.d

build-posix/olconf.mk: build-posix
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/posix/olconf.yaml > $@

build-posix:
	$(call MKDIR, build-posix)

.PHONY: cantil-posix all-posix check-posix coverage-posix
