cy-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
cy-incs := -I$(cy-dir)/include
cy-lib := libcydiom.a
cy-breeconf := python -B $(cy-dir)/breeconf.py $(cy-yamls)

cy-cflags := -Wall
cy-cflags += -Wcast-align
cy-cflags += -Wconversion
cy-cflags += -Wdisabled-optimization
cy-cflags += -Wextra
cy-cflags += -Wlogical-op
cy-cflags += -Wmissing-prototypes
cy-cflags += -Wnested-externs
cy-cflags += -Wpadded
cy-cflags += -Wredundant-decls
cy-cflags += -Wshadow
cy-cflags += -Wstrict-prototypes
cy-cflags += -Wswitch-default
cy-cflags += -Wwrite-strings

all-cy:
ifeq "$(shell $(cy-breeconf))" ""
	$(error "Breeck failure.")
endif
	make _all-cy $(shell $(cy-breeconf))

_all-cy: $(cy-blddir)/$(cy-lib)

$(cy-blddir)/$(cy-lib): $(cy-objs) $(cy-blddir)
	ar rcs $@ $(cy-objs)

$(cy-objs): CFLAGS ?= $(cy-cflags) $(cy-extra-cflags)
$(cy-blddir)/%.o: $(cy-dir)/%.c | $(cy-blddirs)
	$(CC) $(CFLAGS) $(cy-incs) -c -o $@ $<

$(cy-blddirs):
	mkdir -p $@

.PHONY: all-cy _all-cy
