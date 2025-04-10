blddir ?= build
ut-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut-blddir := $(blddir)/$(ut-dir)
ut-objs := $(ut-blddir)/test_algo.o
unity-dir := Unity

run-ut: $(ut-blddir)/app
	$(ub-runcmd) ./$(ut-blddir)/app

$(ut-blddir)/app: $(ut-objs) $(blddir)/$(unity-dir)/unity.o $(ub-lib)
	$(CC) -o $@ $(ut-objs) $(blddir)/$(unity-dir)/unity.o $(ub-lib) $(ub-ldflags)

$(ut-blddir)/%.o: $(ut-dir)/%.c | $(ut-blddir)
	$(CC) $(ub-cflags) -I$(unity-dir)/src -c -o $@ $<

$(blddir)/$(unity-dir)/unity.o: $(unity-dir)/src/unity.c | $(blddir)/$(unity-dir)
	cd $(unity-dir) && git checkout v2.6.1
	$(CC) $(ub-cflags) -I$(unity-dir)/src -c -o $@ $<

$(unity-dir)/src/unity.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(blddir)/$(unity-dir):
	mkdir -p $@

$(ut-blddir):
	mkdir -p $@

.PHONY: run-ut
