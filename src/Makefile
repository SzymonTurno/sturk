cn-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
cn-incs := -I$(cn-dir)/include
cn-lib := libcantil.a
cn-breeconf := python -B $(cn-dir)/breeconf.py $(cn-yamls)

cn-cflags := -Wall
cn-cflags += -Wcast-align
cn-cflags += -Wconversion
cn-cflags += -Wdisabled-optimization
cn-cflags += -Wextra
cn-cflags += -Wlogical-op
cn-cflags += -Wmissing-prototypes
cn-cflags += -Wnested-externs
cn-cflags += -Wpadded
cn-cflags += -Wredundant-decls
cn-cflags += -Wshadow
cn-cflags += -Wstrict-prototypes
cn-cflags += -Wswitch-default
cn-cflags += -Wwrite-strings

all-cn:
ifeq "$(shell $(cn-breeconf))" ""
	$(error "Breeck failure.")
endif
	make _all-cn $(shell $(cn-breeconf))

_all-cn: $(cn-blddir)/$(cn-lib)

$(cn-blddir)/$(cn-lib): $(cn-objs) $(cn-blddir)
	ar rcs $@ $(cn-objs)

$(cn-objs): CFLAGS ?= $(cn-cflags) $(cn-extra-cflags)
$(cn-blddir)/%.o: $(cn-dir)/%.c | $(cn-blddirs)
	$(CC) $(CFLAGS) $(cn-incs) -c -o $@ $<

$(cn-blddirs):
	mkdir -p $@

.PHONY: all-cn _all-cn
