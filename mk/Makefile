include $(BLDDIR)/olconf.mk

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
	./$(test_BLDDIR)/app
	echo No memchecks for Windows. > $@
else
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file="$@" ./$(test_BLDDIR)/app
endif

$(test_BLDDIR)/reports.d:
	$(call MKDIR, $@)

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

$(unity_BLDDIR)/%.o: $(unity_DIR)/%.c | $(unity_BLDDIRS)
	$(CC) $(unity_INCS) -c -o $@ $<

$(unity_DIR)/%.c: $(unity_DIR)
	cd $(unity_DIR) && git reset --hard v2.6.1

$(unity_DIR):
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(unity_BLDDIRS):
	$(call MKDIR, $@)
