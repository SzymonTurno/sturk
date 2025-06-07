sample_BLDDIR := $(BLDDIR)/$(sample_DIR)
sample_INCS := -I$(sample_DIR) $(cantil_INC)
sample_OBJS += $(sample_BLDDIR)/single_thread_pubsub.o
sample_OBJS += $(sample_BLDDIR)/multi_thread_pubsub.o

$(sample_OBJS): CFLAGS += $(cantil_CFLAGS)
$(sample_BLDDIR)/%.o: $(sample_DIR)/%.c | $(sample_BLDDIR)
	$(CC) $(CFLAGS) $(sample_INCS) -c -o $@ $<

$(sample_BLDDIR):
	mkdir -p $@
