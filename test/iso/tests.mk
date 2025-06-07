run-iso_tests:
	make _run-tests cantil_YAMLS="$(test_DIR)/iso/cantil.yaml $(cantil_YAMLS)" \
		test_POSIX_EN="false" BLDDIR="build-iso_tests"

coverage-iso_tests:
	make _coverage BLDDIR="build-iso_tests"

.PHONY: run-iso_tests coverage-iso_tests
