unity_DIR := Unity
unity_BLDDIR := $(BLDDIR)/$(unity_DIR)
unity_BLDDIRS := $(unity_BLDDIR)
unity_BLDDIRS += $(unity_BLDDIR)/src/
unity_BLDDIRS += $(unity_BLDDIR)/extras/fixture/src/
unity_BLDDIRS += $(unity_BLDDIR)/extras/memory/src/
unity_INCS := -I$(unity_DIR)/src
unity_INCS += -I$(unity_DIR)/extras/fixture/src
unity_INCS += -I$(unity_DIR)/extras/memory/src
unity_OBJS := $(unity_BLDDIR)/src/unity.o
unity_OBJS += $(unity_BLDDIR)/extras/fixture/src/unity_fixture.o
unity_OBJS += $(unity_BLDDIR)/extras/memory/src/unity_memory.o

$(unity_BLDDIR)/%.o: $(unity_DIR)/%.c | $(unity_BLDDIRS)
	$(CC) $(unity_INCS) -c -o $@ $<

$(unity_DIR)/%.c: $(unity_DIR)
	cd $(unity_DIR) && git reset --hard v2.6.1

$(unity_DIR):
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(unity_BLDDIRS):
	mkdir -p $@
