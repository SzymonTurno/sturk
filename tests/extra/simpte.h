#ifndef SIMPTE_H
#define SIMPTE_H

#include "sturk/io/buffer.h"
#include "sturk/io/logger.h"
#include "sturk/os/mem.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifndef TEST

#define TEST(group, case) void simpte_##group##_##case (void)

#endif /* TEST */

#ifndef RUN_TEST_CASE

/* clang-format off */
#define RUN_TEST_CASE(group, case)                                             \
	do {                                                                   \
		simpte_##group##_##case();                                     \
		printf("Done\n");                                              \
		st_mem_cleanup();                                              \
	} while (0)
/* clang-format on */

#endif /* RUN_TEST_CASE */

#ifndef TEST_ASSERT_EQUAL_INT

#define TEST_ASSERT_EQUAL_INT(expected, actual) assert((expected) == (actual))

#endif /* TEST_ASSERT_EQUAL_INT */

#ifndef TEST_ASSERT_GREATER_THAN_INT

#define TEST_ASSERT_GREATER_THAN_INT(expected, actual)                         \
	assert((expected) > (actual))

#endif /* TEST_ASSERT_GREATER_THAN_INT */

#ifndef TEST_ASSERT_LESS_OR_EQUAL_INT

#define TEST_ASSERT_LESS_OR_EQUAL_INT(expected, actual)                        \
	assert((expected) <= (actual))

#endif /* TEST_ASSERT_LESS_OR_EQUAL_INT */

#ifndef TEST_ASSERT_EQUAL_PTR

#define TEST_ASSERT_EQUAL_PTR(expected, actual)                                \
	assert((void*)(expected) == (void*)(actual))

#endif /* TEST_ASSERT_EQUAL_PTR */

#ifndef TEST_ASSERT_EQUAL_STRING

#define TEST_ASSERT_EQUAL_STRING(expected, actual)                             \
	assert(strcmp((expected), (actual)) == 0)

#endif /* TEST_ASSERT_EQUAL_STRING */

#ifdef TEST_ASSERT_NOT_EQUAL_INT

#define TEST_ASSERT_NOT_EQUAL_STRING(expected, actual)                         \
	TEST_ASSERT_NOT_EQUAL_INT(0, strcmp((expected), (actual)))

#else /* not defined: TEST_ASSERT_NOT_EQUAL_INT */

#define TEST_ASSERT_NOT_EQUAL_STRING(expected, actual)                         \
	assert(strcmp((expected), (actual)) != 0)

#endif /* TEST_ASSERT_NOT_EQUAL_INT */

#define SIMPTE_IO(ref) simpte_##ref##_io

#define SIMPTE_IO_DCL(ref, bytes)                                              \
	static StIoBuffer simpte_##ref##_buff[iobuffer_calclen(bytes)];        \
	static StIo* SIMPTE_IO(ref)

#define SIMPTE_IO_INIT(ref) SIMPTE_IO(ref) = io_init(simpte_##ref##_buff)

#define SIMPTE_BEGIN()                                                         \
	SIMPTE_IO_DCL(STDOUT, 0);                                              \
	SIMPTE_IO_DCL(STDERR, 0)

#define SIMPTE_GROUP(group)                                                    \
	SIMPTE_IO_DCL(group, 512);                                             \
	TEST_SETUP(group)                                                      \
	{                                                                      \
		SIMPTE_IO_INIT(STDOUT);                                        \
		SIMPTE_IO_INIT(STDERR);                                        \
		SIMPTE_IO_INIT(group);                                         \
		io_putc(SIMPTE_IO(group), IO_EOF);                             \
		SIMPTE_IO_INIT(group);                                         \
		io_setp(SIMPTE_IO(STDOUT), stdout);                            \
		io_setvp(SIMPTE_IO(STDOUT), SAMPLE_FILE_API);                  \
		io_setp(SIMPTE_IO(STDERR), stdout);                            \
		io_setvp(SIMPTE_IO(STDERR), SAMPLE_FILE_API);                  \
		logger_attach(INFO, SIMPTE_IO(STDOUT));                        \
		logger_attach(DEBUG, SIMPTE_IO(STDOUT));                       \
		logger_attach(WARNING, SIMPTE_IO(STDERR));                     \
		logger_attach(ERROR, SIMPTE_IO(STDERR));                       \
		printf("\n");                                                  \
		logger_attach(INFO, SIMPTE_IO(group));                         \
		logger_attach(DEBUG, SIMPTE_IO(group));                        \
		logger_attach(WARNING, SIMPTE_IO(group));                      \
		logger_attach(ERROR, SIMPTE_IO(group));                        \
	}                                                                      \
	TEST_TEAR_DOWN(group)                                                  \
	{                                                                      \
		trace(INFO, "ut", "Done.");                                    \
		logger_cleanup();                                              \
		st_mem_cleanup();                                              \
	}                                                                      \
	TEST_GROUP(group)

#define SIMPTE_GETTRACE(group, index)                                          \
	SimpteGettrace(simpte_##group##_buff, index)

static inline void SimpteMain(int argc, const char** argv, void (*fn)(void))
{
	(void)argc;
	(void)argv;
	printf("Simpte test run 1 of 1\n");
	fn();
	printf("\n\n-----------------------\nOK\n");
}

static inline const char* SimpteGettrace(StIoBuffer* buff, int index)
{
	static char out[256];
	StIo* io = io_init(buff);

	while (index--)
		io_fgets(out, sizeof(out), io);
	return io_fgets(out, sizeof(out), io);
}

#endif /* SIMPTE_H */
