suite1_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
suite1_blddir := $(blddir)/$(suite1_dir)
ut-blddirs += $(suite1_blddir)

suite1_objs := $(suite1_blddir)/main.o
suite1_objs += $(ut1_objs)
suite1_objs += $(ut2_objs)
$(suite1_objs): CFLAGS += $(ub-cflags)

run-suite1:
	make _run-suite1 ub-yamls="$(suite1_dir)/ub.yaml $(ub-yamls)"

_run-suite1: $(suite1_blddir)/app
	$(ub-runcmd) ./$(suite1_blddir)/app

$(suite1_blddir)/app: $(suite1_objs) $(ut-blddir)/unity.o $(ub-lib)
	$(CC) -o $@ $(suite1_objs) $(ut-blddir)/unity.o $(ub-lib) $(ub-ldflags)

.PHONY: run-suite1 _run-suite1
