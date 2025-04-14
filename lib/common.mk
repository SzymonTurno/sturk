cver := $(shell $(ub-getcfg) -s common -p cver)
ifeq ($(cver), gnu)
ub-cflags ?= -std=gnu99
else ifeq ($(cver), iso)
ub-cflags ?= -std=c99 -pedantic
else
$(error Unknown cver: "$(cver)")
endif

bldtype := $(shell $(ub-getcfg) -s common -p build_type)
ifeq ($(bldtype), release)
ub-cflags += -O3
else ifeq ($(bldtype), debug)
ub-cflags += -g
else ifeq ($(bldtype), coverage)
ub-blddirs += $(blddir)/analysis.d
ub-cflags += -g
ub-gcov := -fprofile-arcs -ftest-coverage
ub-ldflags += -lgcov --coverage
else
$(error Unknown build type: "$(bldtype)")
endif

runmode := $(shell $(ub-getcfg) -s common -p run_mode)
ifeq ($(runmode), valgrind)
ub-blddirs += $(blddir)/analysis.d
ub-runcmd := valgrind
ub-runcmd += --leak-check=full --show-leak-kinds=all --track-origins=yes
ub-runcmd += --log-file="$(blddir)/analysis.d/valgrind.info"
else ifneq ($(runmode), none)
$(error Unknown run mode: "$(runmode)")
endif

ub-cflags += -Wall -Wextra
ub-cflags += -Wbad-function-cast
ub-cflags += -Wcast-align
ub-cflags += -Wconversion
ub-cflags += -Wdisabled-optimization
ub-cflags += -Wlogical-op
ub-cflags += -Wmissing-prototypes
ub-cflags += -Wnested-externs
ub-cflags += -Wpadded
ub-cflags += -Wredundant-decls
ub-cflags += -Wshadow
ub-cflags += -Wstrict-prototypes
ub-cflags += -Wswitch-default
ub-cflags += -Wwrite-strings
ub-cflags += $(ub-incs)
