ifeq ($(OS),Windows_NT)
	MKDIR = mkdir $(subst /,\,$(1))
else
	MKDIR = mkdir -p $(1)
endif

all: $(cantil_BLDDIR)/libcantil.a

$(test_BLDDIR)/reports.d/coverage.d: $(test_BLDDIR)/reports.d/coverage.info
ifeq ($(OS),Windows_NT)
	$(call MKDIR, $@)
else
	genhtml $(test_BLDDIR)/reports.d/coverage.info --show-details \
		--output-directory $@
endif

$(test_BLDDIR)/reports.d/coverage.info: $(test_BLDDIR)/reports.d/valgrind.info
ifeq ($(OS),Windows_NT)
	echo No coverage for Windows. > $@
else
	lcov --capture --directory ./$(cantil_BLDDIR) --output-file $@
endif

$(test_BLDDIR)/reports.d/valgrind.info: $(test_BLDDIR)/app $(test_BLDDIR)/reports.d
ifeq ($(OS),Windows_NT)
	$(tools_DIR)/picky.sh ./$(test_BLDDIR)/app
	echo No memchecks for Windows. > $@
else
	$(tools_DIR)/picky.sh valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --error-exitcode=1 --log-file="$@" \
		./$(test_BLDDIR)/app
endif

$(test_BLDDIR)/reports.d:
	$(call MKDIR, $@)

test_DEPS:=$(test_OBJS)
test_DEPS+=$(unity_OBJS)
test_DEPS+=$(sample_OBJS)
test_DEPS+=$(cantil_BLDDIR)/libcantil.a
$(test_BLDDIR)/app: $(test_DEPS)
	$(CC) -o $@ $(test_DEPS) -lgcov --coverage

$(test_BLDDIR)/%.o: $(test_DIR)/%.c | $(test_BLDDIR) $(unity_DIR)
	$(CC) $(test_CFLAGS) $(unity_INCS) $(sample_INC) $(cantil_INC) -c -o $@ $<

$(test_BLDDIR):
	$(call MKDIR, $@)

$(sample_BLDDIR)/%.o: $(sample_DIR)/%.c | $(sample_BLDDIR)
	$(CC) $(cantil_CFLAGS) $(sample_INC) $(cantil_INC) -c -o $@ $<

$(sample_BLDDIR):
	$(call MKDIR, $@)

$(cantil_BLDDIR)/libcantil.a: $(cantil_OBJS)
	ar rcs $@ $(cantil_OBJS)

$(cantil_BLDDIR)/%.o: $(cantil_DIR)/%.c | $(cantil_BLDDIRS)
	$(CC) $(cantil_CFLAGS) $(cantil_EXTRA_CFLAGS) $(cantil_INC) -c -o $@ $<

$(cantil_BLDDIRS):
	$(call MKDIR, $@)

$(unity_DIR)/%.o: $(unity_DIR)/%.c
	$(CC) $(unity_INCS) -c -o $@ $<

$(unity_DIR)/%.c: $(unity_DIR)
	cd $(unity_DIR) && git reset --hard v2.6.1

$(unity_DIR):
	git clone https://github.com/ThrowTheSwitch/Unity.git
	cd $(unity_DIR) && git reset --hard v2.6.1

.PHONY: all
