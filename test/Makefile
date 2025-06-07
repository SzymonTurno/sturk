test_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
test_BLDDIR := $(BLDDIR)/$(test_DIR)
test_OBJS := $(test_BLDDIR)/main.o
test_POSIX_EN ?= false
test_RPTDIR := $(BLDDIR)/src/analysis.d

include $(test_DIR)/unity.mk
include $(test_DIR)/sample.mk
include $(test_DIR)/iso/tests.mk
include $(test_DIR)/posix/tests.mk

_run-tests: $(test_BLDDIR)/app
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file="$(test_RPTDIR)/valgrind.info" \
		./$(test_BLDDIR)/app

$(test_BLDDIR)/app: $(test_OBJS) $(sample_OBJS) $(unity_OBJS)
	make all-cantil cantil_YAMLS=$(cantil_YAMLS)
	$(CC) -o $@ $(test_OBJS) $(sample_OBJS) $(unity_OBJS) \
		$(BLDDIR)/$(cantil_DIR)/$(cantil_LIB) -lgcov --coverage

ifeq ($(test_POSIX_EN),true)
$(test_OBJS): CFLAGS += -DPOSIX_TESTS_ON=1
else ifeq ($(test_POSIX_EN),false)
$(test_OBJS): CFLAGS += -DPOSIX_TESTS_ON=0
else
$(error Unknown test_POSIX_EN value: $(test_POSIX_EN))
endif
$(test_BLDDIR)/%.o: $(test_DIR)/%.c | $(test_BLDDIR) $(unity_OBJS)
	$(CC) $(CFLAGS) $(unity_INCS) $(sample_INCS) -c -o $@ $<

$(test_BLDDIR):
	mkdir -p $@

_coverage: $(test_RPTDIR)/coverage.d

$(test_RPTDIR)/coverage.d: $(test_RPTDIR)/coverage.info
	genhtml $(test_RPTDIR)/coverage.info --show-details --output-directory $@

$(test_RPTDIR)/coverage.info: $(test_RPTDIR)
	lcov --capture --directory ./$(BLDDIR) --output-file $@

.PHONY: _run-tests _coverage
