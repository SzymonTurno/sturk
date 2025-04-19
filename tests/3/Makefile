test3_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
test3_blddir := $(blddir)/$(test3_dir)
ut-blddirs += $(test3_blddir)

ut-objs_posix += $(test3_blddir)/middleware.o $(test3_blddir)/test.o

$(test3_blddir)/test.o: CFLAGS += -DEXPECTED_OUT="\"$(test3_dir)/expected.out\""
$(test3_blddir)/test.o: CFLAGS += -DACTUAL_OUT="\"$(test3_dir)/actual.out\""
