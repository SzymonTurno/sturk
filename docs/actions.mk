MARKDOWN_DOCS := ./build/docs/README.md
MARKDOWN_DOCS += ./build/docs/src/broker/README.md
MARKDOWN_DOCS += ./build/docs/include/vertegs/README.md
HEADER_DOCS := ./build/docs/st
HEADER_DOCS += ./build/docs/sturk
HEADER_DOCS += ./build/docs/vertegs

public: build/docs/public
	cp -r build/docs/public public

build/docs/public: $(HEADER_DOCS) $(MARKDOWN_DOCS)
ifeq ($(PROJECT_NUMBER),)
	cd build/docs && (cat ../../docs/Doxyfile ; \
	echo "PROJECT_NUMBER=$(shell git describe --tags --abbrev=0 \
		| sed 's/v//')") | doxygen -
else
	cd build/docs && (cat ../../docs/Doxyfile ; \
	echo "PROJECT_NUMBER=$(PROJECT_NUMBER)") | doxygen -
endif

build/docs/%.md: %.md | $(HEADER_DOCS)
	./tools/prepdocs.pl $@

build/docs/st: build/docs include/st
	cp -r include/st $@

build/docs/sturk: build/docs include/sturk
	cp -r include/sturk $@

build/docs/vertegs: build/docs include/vertegs
	cp -r include/vertegs $@

build/docs: build
	$(call MKDIR, $@)
