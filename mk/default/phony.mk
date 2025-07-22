cantil-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build build/src/libcantil.a

all-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build build/tests/app

check-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build \
		build/tests/reports.d/valgrind.info

build/olconf.mk: build
	$(PYTHON) ./tools/olgite.py ./olconf.py > $@

build:
	$(call MKDIR, build)

.PHONY: cantil-default all-default check-default
