ub-cverstr := $(shell $(ub-getcfg) -s common -p cver)

ifeq ($(ub-cverstr), gnu)
ub-cver := -std=gnu99
else ifeq ($(ub-cverstr), iso)
ub-cver := -std=c99 -pedantic
else
$(error Unknown cver: "$(ub-cver)")
endif

ub-dbgstr := $(shell $(ub-getcfg) -s common -p debug)

ifeq ($(ub-dbgstr), on)
ub-runcmd := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
ub-cflags += -g
else ifneq ($(ub-dbgstr), off)
$(error Unknown debug: "$(ub-dbgstr)")
endif

ub-cflags := -O3 -Wall -Wextra
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
