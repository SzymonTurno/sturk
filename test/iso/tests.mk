run-iso_tests: build-iso_tests
	make _run-tests BLDDIR="build-iso_tests"

build-iso_tests:
	make build-iso_tests/$(test_DIR)/app \
		cantil_YAMLS="$(test_DIR)/iso/cantil.yaml $(cantil_YAMLS)" \
		test_MULTITHREADING_EN="false" \
		BLDDIR="build-iso_tests"

coverage-iso_tests:
	make _coverage BLDDIR="build-iso_tests"

.PHONY: run-iso_tests coverage-iso_tests
