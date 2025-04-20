suite2_dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
suite2_blddir := $(blddir)/$(suite2_dir)
ut-blddirs += $(suite2_blddir)

suite2_objs := $(suite2_blddir)/main.o
suite2_objs += $(ut1_objs)
suite2_objs += $(ut3_objs)
$(suite2_objs): CFLAGS += $(ub-cflags)

run-suite2:
	make _run-suite2 ub-yamls="$(suite2_dir)/ub.yaml $(ub-yamls)"

_run-suite2: $(suite2_blddir)/app
	$(ub-runcmd) ./$(suite2_blddir)/app

$(suite2_blddir)/app: $(suite2_objs) $(ut-blddir)/unity.o $(ub-lib)
	$(CC) -o $@ $(suite2_objs) $(ut-blddir)/unity.o $(ub-lib) $(ub-ldflags)

.PHONY: run-suite2 _run-suite2
