all-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso build-iso/src/libcantil.a

test-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso \
		build-iso/test/reports.d/valgrind.info

coverage-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso \
		build-iso/test/reports.d/coverage.d

build-iso/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build-iso/.*),)
	$(error configuration changed, to rebuild: remove "build-iso/")
endif
	$(MKDIR) build-iso
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/iso/olconf.yaml > $@

.PHONY: all-iso test-iso coverage-iso
