test_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
test_blddir := $(blddir)/$(test_dir)
test_objs := $(test_blddir)/main.o
blddir ?= build
unity_dir := Unity
unity_objs := $(test_blddir)/unity.o
unity_objs += $(test_blddir)/unity_fixture.o
unity_objs += $(test_blddir)/unity_memory.o
unity_incs := -I$(unity_dir)/src
unity_incs += -I$(unity_dir)/extras/fixture/src
unity_incs += -I$(unity_dir)/extras/memory/src

run-iso_tests:
	make _run-tests ub-yamls="$(test_dir)/iso.yaml $(ub-yamls)"

run-posix_tests:
	make _run-tests ub-yamls="$(test_dir)/posix.yaml $(ub-yamls)"

_run-tests: $(test_blddir)/app
	$(ub-runcmd) ./$(test_blddir)/app

$(test_blddir)/app: $(test_objs) $(sample_objs) $(unity_objs) $(ub-lib)
	$(CC) -o $@ $(test_objs) $(sample_objs) $(unity_objs) $(ub-lib) $(ub-ldflags)

$(test_objs): CFLAGS += -DEXPECTED_OUT="\"$(test_dir)/expected.out\""
$(test_objs): CFLAGS += -DACTUAL_OUT="\"$(test_dir)/actual.out\""
$(test_blddir)/%.o: $(test_dir)/%.c | $(test_blddir)
	$(CC) $(CFLAGS) $(unity_incs) $(sample_incs) -c -o $@ $<

$(sample_objs): CFLAGS += $(ub-cflags)
$(sample_blddir)/%.o: $(sample_dir)/%.c | $(sample_blddir)
	$(CC) $(CFLAGS) -c -o $@ $<

$(test_blddir)/unity.o: $(unity_dir)/src/unity.c | $(test_blddir)
	cd $(unity_dir) && git checkout v2.6.1
	$(CC) $(unity_incs) -c -o $@ $<

$(test_blddir)/unity_fixture.o: $(unity_dir)/extras/fixture/src/unity_fixture.c | $(test_blddir)
	cd $(unity_dir) && git checkout v2.6.1
	$(CC) $(unity_incs) -c -o $@ $<

$(test_blddir)/unity_memory.o: $(unity_dir)/extras/memory/src/unity_memory.c | $(test_blddir)
	cd $(unity_dir) && git checkout v2.6.1
	$(CC) $(unity_incs) -c -o $@ $<

$(unity_dir)/%.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(test_blddir):
	mkdir -p $@

.PHONY: run-iso_tests run-posix_tests _run-tests
