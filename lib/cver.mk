ub-cverstr := $(shell $(ub-getcfg) -s constants -p cver)

ifeq ($(ub-cverstr), gnu)
ub-cver := -std=gnu11
else ifeq ($(ub-cverstr), iso)
ub-cver := -std=c99 -pedantic
else
$(error Unknown cver: "$(ub-cver)")
endif
