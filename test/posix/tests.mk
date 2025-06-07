run-posix_tests:
	make _run-tests cantil_YAMLS="$(test_DIR)/posix/cantil.yaml $(cantil_YAMLS)" \
		test_POSIX_EN="true" BLDDIR="build-posix_tests"

coverage-posix_tests:
	make _coverage BLDDIR="build-posix_tests"

.PHONY: run-posix_tests coverage-posix_tests
