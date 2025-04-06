blddir ?= build
ut-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut-gnu-target := $(blddir)/$(ut-dir)/ut_app_gnu
ut-iso-target := $(blddir)/$(ut-dir)/ut_app_iso
ut-objs := $(blddir)/$(ut-dir)/test_algo.o
unity-dir := Unity

run-ut-gnu: $(ut-gnu-target)
	./$(ut-gnu-target)

run-ut-iso: $(ut-iso-target)
	./$(ut-iso-target)

$(ut-objs): CC = gcc
$(ut-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs) -I$(unity-dir)/src

$(ut-gnu-target): $(blddir)/$(ut-dir) $(blddir)/$(ut-dir)/unity.o $(ut-objs)
	make all-ub
	gcc -o $@ $(blddir)/$(ut-dir)/unity.o $(ut-objs) $(ub-lib)

$(ut-iso-target): $(blddir)/$(ut-dir) $(blddir)/$(ut-dir)/unity.o $(ut-objs)
	make all-ub ub-yamls=$(ut-dir)/iso.yaml
	gcc -o $@ $(blddir)/$(ut-dir)/unity.o $(ut-objs) $(ub-lib)

$(blddir)/$(ut-dir)/unity.o: $(unity-dir)/src/unity.c | $(blddir)/$(ut-dir)
	cd $(unity-dir) && git checkout v2.6.1
	$(CC) $(CFLAGS) -c -o $@ $<

$(blddir)/$(ut-dir)/%.o: $(ut-dir)/%.c | $(blddir)/$(ut-dir)
	$(CC) $(CFLAGS) -c -o $@ $<

$(unity-dir)/src/unity.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(blddir)/$(ut-dir):
	mkdir -p $@

.PHONY: run-ut-gnu run-ut-iso
