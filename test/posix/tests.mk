run-posix_tests: build-posix_tests
	make _run-tests BLDDIR="build-posix_tests"

build-posix_tests:
	make build-posix_tests/$(test_DIR)/app \
		cantil_YAMLS="$(test_DIR)/posix/cantil.yaml $(cantil_YAMLS)" \
		test_MULTITHREADING_EN="true" \
		BLDDIR="build-posix_tests"

coverage-posix_tests:
	make _coverage BLDDIR="build-posix_tests"

.PHONY: run-posix_tests coverage-posix_tests
