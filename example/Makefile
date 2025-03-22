example-dir := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
example-build := build/$(example-dir)
example-target := $(example-build)/example
example-objs := $(example-dir)/example.o $(example-dir)/middleware.o

run-example: $(example-target)
	./$(example-target)

$(example-target): $(example-objs) $(ub-lib) $(example-build)
	gcc -o $@ $(example-objs) $(ub-lib)
	mv $(example-objs) $(example-build)/

$(example-objs): CC = gcc
$(example-objs): CFLAGS += -std=c99 -pedantic $(ub-cflags) $(ub-incs)

$(example-build):
	mkdir -p $@

clean-example:
	rm -f $(example-objs)

.PHONY: clean-example
