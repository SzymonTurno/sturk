cantil-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build build/src/libcantil.a

all-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build build/tests/app

check-default: build/olconf.mk
	$(MAKE) -f ./mk/Makefile BLDDIR=build \
		build/tests/reports.d/valgrind.info

build/olconf.mk: $(cantil_OLCONF)
ifneq ($(wildcard build/.*),)
	$(error configuration changed, to rebuild: remove "build/")
endif
	$(call MKDIR, build)
	$(PYTHON) ./tools/olgite.py ./olconf.py > $@

.PHONY: cantil-default all-default check-default
