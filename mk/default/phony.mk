all-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build build/src/libcantil.a

test-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build \
		build/test/reports.d/valgrind.info

build/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build/.*),)
	$(error configuration changed, to rebuild: remove "build/")
endif
	$(MKDIR) build
	$(PYTHON) ./tools/olgite.py ./olconf.py > $@

.PHONY: all-default test-default
