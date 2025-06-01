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
ut-posix ?= off

run-iso_tests:
	make _run-tests cn-yamls="$(test_dir)/iso.yaml $(cn-yamls)" ut-posix="off"

run-posix_tests:
	make _run-tests cn-yamls="$(test_dir)/posix.yaml $(cn-yamls)" ut-posix="on"

_run-tests: $(test_blddir)/app
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file="$(rptdir)/valgrind.info" \
		./$(test_blddir)/app

$(test_blddir)/app: $(test_objs) $(sample_objs) $(unity_objs)
	make all-cn cn-yamls=$(cn-yamls)
	$(CC) -o $@ $(test_objs) $(sample_objs) $(unity_objs) \
		build/$(cn-dir)/$(cn-lib) -lgcov --coverage

ifeq ($(ut-posix),on)
$(test_objs): CFLAGS += -DPOSIX_TESTS_ON=1
else ifeq ($(ut-posix),off)
$(test_objs): CFLAGS += -DPOSIX_TESTS_ON=0
else
$(error Unkown ut-posix value: $(ut-posix))
endif
$(test_objs): CFLAGS += -DEXPECTED_OUT="\"$(test_dir)/expected.out\""
$(test_objs): CFLAGS += -DACTUAL_OUT="\"$(test_dir)/actual.out\""
$(test_blddir)/%.o: $(test_dir)/%.c | $(unity_objs)
	$(CC) $(CFLAGS) $(unity_incs) $(sample_incs) -c -o $@ $<

$(sample_objs): CFLAGS += $(cn-cflags)
$(sample_blddir)/%.o: $(sample_dir)/%.c | $(sample_blddir)
	$(CC) $(CFLAGS) $(cn-incs) -c -o $@ $<

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
