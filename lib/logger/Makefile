logger_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
logger_blddir := $(blddir)/$(logger_dir)
ub-blddirs += $(logger_blddir)

getcfg := $(ub-getcfg) $(logger_dir)/default.yaml -s logger

debug := $(shell $(getcfg) -p debug)
ifeq ($(debug), on)
ub-cflags += -DUB_DEBUG_EN=1
else ifneq (debug, off)
$(error Unknown debug mode: "$(debug)")
endif

info := $(shell $(getcfg) -p info)
ifeq ($(info), on)
ub-cflags += -DUB_INFO_EN=1
else ifneq (info, off)
$(error Unknown info mode: "$(info)")
endif

warning := $(shell $(getcfg) -p warning)
ifeq ($(warning), on)
ub-cflags += -DUB_WARNING_EN=1
else ifneq (warning, off)
$(error Unknown warning mode: "$(warning)")
endif

error := $(shell $(getcfg) -p error)
ifeq ($(error), on)
ub-cflags += -DUB_ERROR_EN=1
else ifneq (error, off)
$(error Unknown error mode: "$(error)")
endif

ub-objs += $(logger_blddir)/logger.o
