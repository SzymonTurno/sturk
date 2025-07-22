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

build-iso/olconf.mk: build-iso
	$(PYTHON) ./tools/olgite.py ./olconf.py ./mk/iso/olconf.yaml > $@

build-iso:
	$(call MKDIR, build-iso)

.PHONY: cantil-iso all-iso check-iso coverage-iso
