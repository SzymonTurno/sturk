cantil-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso build-iso/src/libcantil.a

all-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso build-iso/tests/app

check-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso \
		build-iso/tests/reports.d/valgrind.info

coverage-iso: build-iso/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build-iso \
		build-iso/tests/reports.d/coverage.d

build-iso/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build-iso/.*),)
	$(error configuration changed, to rebuild: remove "build-iso/")
endif
	$(call MKDIR, build-iso)
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/iso/olconf.yaml > $@

.PHONY: cantil-iso all-iso check-iso coverage-iso
