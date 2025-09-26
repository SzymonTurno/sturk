MARKDOWN_DOCS := ./build/docs/README.md
MARKDOWN_DOCS += ./build/docs/src/broker/README.md
MARKDOWN_DOCS += ./build/docs/src/algo/README.md
MARKDOWN_DOCS += ./build/docs/src/logger/README.md
MARKDOWN_DOCS += ./build/docs/src/osal/README.md
MARKDOWN_DOCS += ./build/docs/src/basis/README.md
MARKDOWN_DOCS += ./build/docs/include/vertegs/README.md
HEADER_DOCS := ./build/docs/st
HEADER_DOCS += ./build/docs/sturk
HEADER_DOCS += ./build/docs/vertegs

public: build/docs/public
	cp -r build/docs/public public

build/docs/public: $(HEADER_DOCS) $(MARKDOWN_DOCS)
ifeq ($(RELEASE_VERSION),)
	@echo RELEASE_VERSION is empty
	cd build/docs && (cat ../../docs/Doxyfile ; \
	echo "PROJECT_NUMBER=$(shell git describe --tags --abbrev=0 \
		| sed 's/v//')") | doxygen -
else
	@echo RELEASE_VERSION is $(RELEASE_VERSION)
	cd build/docs && (cat ../../docs/Doxyfile ; \
	echo "PROJECT_NUMBER=$(subst v,,$(RELEASE_VERSION))") | doxygen -
endif

build/docs/%.md: %.md | $(HEADER_DOCS)
	./tools/prepdocs.pl $@

build/docs/st: include/st | build/docs
	cp -r include/st $@

build/docs/sturk: include/sturk | build/docs
	cp -r include/sturk $@

build/docs/vertegs: include/vertegs | build/docs
	cp -r include/vertegs $@

build/docs: build
	$(call MKDIR, $@)
