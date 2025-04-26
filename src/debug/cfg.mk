getcfg := $(ub-getcfg) $(debug_dir)/default.yaml -s debug

log := $(shell $(getcfg) -p log)
ifeq ($(log), on)
ub-cflags += -DUB_LOG_EN=1
else ifneq (debug, off)
$(error Unknown log mode: "$(log)")
endif

exceptions := $(shell $(getcfg) -p exceptions)
ifeq ($(exceptions), on)
ub-cflags += -DUB_EXCEPTIONS_EN=1
else ifneq (exceptions, off)
$(error Unknown exceptions mode: "$(exceptions)")
endif
