sample_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
sample_blddir := $(blddir)/$(sample_dir)
sample_incs := -I$(sample_dir) $(cy-incs)
sample_objs := $(sample_blddir)/algo.o
sample_objs += $(sample_blddir)/single_thread_pubsub.o
sample_objs += $(sample_blddir)/multi_thread_pubsub.o
sample_objs += $(sample_blddir)/snapshot.o

$(sample_blddir):
	mkdir -p $@
