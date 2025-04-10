ub-cverstr := $(shell $(ub-getcfg) -s common -p cver)
ifeq ($(ub-cverstr), gnu)
ub-cflags := -std=gnu99
else ifeq ($(ub-cverstr), iso)
ub-cflags := -std=c99 -pedantic
else
$(error Unknown cver: "$(ub-cverstr)")
endif

ub-modestr := $(shell $(ub-getcfg) -s common -p mode)
ifeq ($(ub-modestr), release)
ub-cflags += -O3
else ifeq ($(ub-modestr), debug)
ub-cflags += -g
else ifeq ($(ub-modestr), analysis)
ub-cflags += -g
ub-runcmd := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
ub-runcmd += --log-file="$(blddir)/analysis.d/valgrind.info"
ub-blddirs += $(blddir)/analysis.d
ub-gcov := -fprofile-arcs -ftest-coverage
ub-ldflags += -lgcov --coverage
else
$(error Unknown mode: "$(ub-modestr)")
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
