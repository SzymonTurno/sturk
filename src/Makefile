cantil_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
cantil_INC := -I$(cantil_DIR)/include
cantil_LIB := libcantil.a
cantil_CFLAGS := -fanalyzer
cantil_CFLAGS += -Wall
cantil_CFLAGS += -Wcast-align
cantil_CFLAGS += -Wconversion
cantil_CFLAGS += -Wdisabled-optimization
cantil_CFLAGS += -Wextra
cantil_CFLAGS += -Wlogical-op
cantil_CFLAGS += -Wmissing-prototypes
cantil_CFLAGS += -Wnested-externs
cantil_CFLAGS += -Wpadded
cantil_CFLAGS += -Wredundant-decls
cantil_CFLAGS += -Wshadow
cantil_CFLAGS += -Wstrict-prototypes
cantil_CFLAGS += -Wswitch-default
cantil_CFLAGS += -Wwrite-strings

all-cantil:
ifeq "$(shell python -B $(cantil_DIR)/olconf.py $(cantil_YAMLS))" ""
	$(error "Olgite failure.")
endif
	make _all-cantil $(shell python -B $(cantil_DIR)/olconf.py $(cantil_YAMLS))

_all-cantil: $(cantil_BLDDIR)/$(cantil_LIB)

$(cantil_BLDDIR)/$(cantil_LIB): $(cantil_OBJS) $(cantil_BLDDIR)
	ar rcs $@ $(cantil_OBJS)

$(cantil_OBJS): CFLAGS ?= $(cantil_CFLAGS) $(cantil_EXTRA_CFLAGS)
$(cantil_BLDDIR)/%.o: $(cantil_DIR)/%.c | $(cantil_BLDDIRS)
	$(CC) $(CFLAGS) $(cantil_INC) -c -o $@ $<

$(cantil_BLDDIRS):
	mkdir -p $@

.PHONY: all-cantil _all-cantil
