all-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix build-posix/src/libcantil.a

test-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix \
		build-posix/test/reports.d/valgrind.info

coverage-posix: build-posix/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-posix \
		build-posix/test/reports.d/coverage.d

build-posix/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build-posix/.*),)
	$(error configuration changed, to rebuild: remove "build-posix/")
endif
	$(MKDIR) build-posix
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/posix/olconf.yaml > $@

.PHONY: all-posix test-posix coverage-posix
