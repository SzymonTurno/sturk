blddir ?= build
unity-dir := Unity
ut-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut-blddir := $(blddir)/$(ut-dir)
ut-blddirs += $(ut-blddir)

include $(ut-dir)/ut1/Makefile
include $(ut-dir)/ut2/Makefile
include $(ut-dir)/ut3/Makefile
include $(ut-dir)/suite1/Makefile
include $(ut-dir)/suite2/Makefile

$(ut-blddir)/%.o: $(ut-dir)/%.c | $(ut-blddirs)
	$(CC) $(CFLAGS) -I$(unity-dir)/src -c -o $@ $<

$(ut-blddir)/unity.o: $(unity-dir)/src/unity.c | $(ut-blddir)
	cd $(unity-dir) && git checkout v2.6.1
	$(CC) -I$(unity-dir)/src -c -o $@ $<

$(unity-dir)/src/unity.c:
	git clone https://github.com/ThrowTheSwitch/Unity.git

$(ut-blddirs):
	mkdir -p $@
