ut2_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
ut2_blddir := $(blddir)/$(ut2_dir)
ut-blddirs += $(ut2_blddir)

ut2_objs += $(ut2_blddir)/middleware.o $(ut2_blddir)/test.o

$(ut2_blddir)/test.o: CFLAGS += -DEXPECTED_OUT="\"$(ut2_dir)/expected.out\""
$(ut2_blddir)/test.o: CFLAGS += -DACTUAL_OUT="\"$(ut2_dir)/actual.out\""
