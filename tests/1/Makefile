test1_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
test1_blddir := $(blddir)/$(test1_dir)
ut-blddirs += $(test1_blddir)

ut-objs_iso += $(test1_blddir)/middleware.o $(test1_blddir)/test.o

$(test1_blddir)/test.o: CFLAGS += -DEXPECTED_OUT="\"$(test1_dir)/expected.out\""
$(test1_blddir)/test.o: CFLAGS += -DACTUAL_OUT="\"$(test1_dir)/actual.out\""
