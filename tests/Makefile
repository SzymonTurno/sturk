blddir ?= build
unity-dir := Unity
ut-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut-blddir := $(blddir)/$(ut-dir)
ut-blddirs += $(ut-blddir)

include $(ut-dir)/1/Makefile
include $(ut-dir)/2/Makefile
include $(ut-dir)/3/Makefile

run-suite_posix:
	make _run-suite_posix ub-yamls="$(ut-dir)/posix.yaml $(ub-yamls)"

_run-suite_posix: $(ut-blddir)/app_posix
	$(ub-runcmd) ./$(ut-blddir)/app_posix

objs_posix := $(ut-blddir)/suite_posix.o $(ut-objs) $(ut-objs_posix)
$(ut-blddir)/app_posix: $(objs_posix) $(ut-blddir)/unity.o $(ub-lib)
	$(CC) -o $@ $(objs_posix) $(ut-blddir)/unity.o $(ub-lib) $(ub-ldflags)

run-suite_iso:
	make _run-suite_iso ub-yamls="$(ut-dir)/iso.yaml $(ub-yamls)"

_run-suite_iso: $(ut-blddir)/app_iso
	$(ub-runcmd) ./$(ut-blddir)/app_iso

objs_iso := $(ut-blddir)/suite_iso.o $(ut-objs) $(ut-objs_iso)
$(ut-blddir)/app_iso: $(objs_iso) $(ut-blddir)/unity.o $(ub-lib)
	$(CC) -o $@ $(objs_iso) $(ut-blddir)/unity.o $(ub-lib) $(ub-ldflags)

$(ut-objs): CFLAGS += $(ub-cflags)
$(ut-objs_iso): CFLAGS += $(ub-cflags)
$(ut-objs_posix): CFLAGS += $(ub-cflags)
$(ut-blddir)/suite_iso.o: CFLAGS += $(ub-cflags)
$(ut-blddir)/suite_posix.o: CFLAGS += $(ub-cflags)
$(ut-blddir)/%.o: $(ut-dir)/%.c | $(ut-blddirs)
	$(CC) $(CFLAGS) -I$(unity-dir)/src -c -o $@ $<

$(ut-blddir)/unity.o: $(unity-dir)/src/unity.c | $(ut-blddir)
	cd $(unity-dir) && git checkout v2.6.1
	$(CC) -I$(unity-dir)/src -c -o $@ $<

$(unity-dir)/src/unity.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(ut-blddirs):
	mkdir -p $@

.PHONY: run-suite_iso _run-suite_iso run-suite_posix _run-suite_posix
