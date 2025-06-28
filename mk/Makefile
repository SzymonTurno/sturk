include $(BLDDIR)/olconf.mk

$(test_BLDDIR)/analysis.d/coverage.d: $(test_BLDDIR)/analysis.d/coverage.info
	genhtml $(test_BLDDIR)/analysis.d/coverage.info --show-details \
		--output-directory $@

$(test_BLDDIR)/analysis.d/coverage.info: $(test_BLDDIR)/analysis.d/valgrind.info
	lcov --capture --directory ./$(cantil_BLDDIR) --output-file $@

$(test_BLDDIR)/analysis.d/valgrind.info: $(test_BLDDIR)/app $(test_BLDDIR)/analysis.d
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file="$@" ./$(test_BLDDIR)/app

$(test_BLDDIR)/analysis.d:
	$(MKDIR) $@

$(test_BLDDIR)/app: $(test_DEPS)
	$(CC) -o $@ $(test_DEPS) -lgcov --coverage

$(test_BLDDIR)/%.o: $(test_DIR)/%.c | $(test_BLDDIR) $(unity_DIR)
	$(CC) $(test_CFLAGS) $(unity_INCS) $(sample_INC) $(cantil_INC) -c -o $@ $<

$(test_BLDDIR):
	$(MKDIR) $@

$(sample_BLDDIR)/%.o: $(sample_DIR)/%.c | $(sample_BLDDIR)
	$(CC) $(cantil_CFLAGS) $(sample_INC) $(cantil_INC) -c -o $@ $<

$(sample_BLDDIR):
	$(MKDIR) $@

$(cantil_BLDDIR)/libcantil.a: $(cantil_OBJS)
	ar rcs $@ $(cantil_OBJS)

$(cantil_BLDDIR)/%.o: $(cantil_DIR)/%.c | $(cantil_BLDDIRS)
	$(CC) $(cantil_CFLAGS) $(cantil_EXTRA_CFLAGS) $(cantil_INC) -c -o $@ $<

$(cantil_BLDDIRS):
	$(MKDIR) $@

$(unity_BLDDIR)/%.o: $(unity_DIR)/%.c | $(unity_BLDDIRS)
	$(CC) $(unity_INCS) -c -o $@ $<

$(unity_DIR)/%.c: $(unity_DIR)
	cd $(unity_DIR) && git reset --hard v2.6.1

$(unity_DIR):
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(unity_BLDDIRS):
	$(MKDIR) $@
