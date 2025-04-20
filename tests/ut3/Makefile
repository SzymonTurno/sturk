ut3_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut3_blddir := $(blddir)/$(ut3_dir)
ut-blddirs += $(ut3_blddir)

ut3_objs += $(ut3_blddir)/middleware.o $(ut3_blddir)/test.o

$(ut3_blddir)/test.o: CFLAGS += -DEXPECTED_OUT="\"$(ut3_dir)/expected.out\""
$(ut3_blddir)/test.o: CFLAGS += -DACTUAL_OUT="\"$(ut3_dir)/actual.out\""
