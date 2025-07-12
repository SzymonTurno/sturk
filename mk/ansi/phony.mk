all-ansi: build-ansi/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-ansi build-ansi/src/libcantil.a

check-ansi: build-ansi/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-ansi \
		build-ansi/tests/reports.d/valgrind.info

coverage-ansi: build-ansi/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-ansi \
		build-ansi/tests/reports.d/coverage.d

build-ansi/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build-ansi/.*),)
	$(error configuration changed, to rebuild: remove "build-ansi/")
endif
	$(call MKDIR, build-ansi)
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/ansi/olconf.yaml > $@

.PHONY: all-ansi check-ansi coverage-ansi
