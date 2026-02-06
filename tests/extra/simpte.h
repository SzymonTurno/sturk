#ifndef SIMPTE_H
#define SIMPTE_H

#include "sturk/io/api.h"
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
	assert((expected) < (actual))

#endif /* TEST_ASSERT_GREATER_THAN_INT */

#ifndef TEST_ASSERT_GREATER_OR_EQUAL_INT

#define TEST_ASSERT_GREATER_OR_EQUAL_INT(expected, actual)                     \
	assert((expected) <= (actual))

#endif /* TEST_ASSERT_GREATER_OR_EQUAL_INT */

#ifndef TEST_ASSERT_LESS_THAN_INT

#define TEST_ASSERT_LESS_THAN_INT(expected, actual)                            \
	assert((expected) > (actual))

#endif /* TEST_ASSERT_LESS_THAN_INT */

#ifndef TEST_ASSERT_LESS_OR_EQUAL_INT

#define TEST_ASSERT_LESS_OR_EQUAL_INT(expected, actual)                        \
	assert((expected) >= (actual))

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

#define SIMPTE_BEGIN()                                                         \
	static struct StIo* SIMPTE_IO(STDOUT);                                 \
	static struct StIo* SIMPTE_IO(STDERR)

#define SIMPTE_GROUP(group)                                                    \
	static struct StIo SIMPTE_IO(group)[iocontig_calclen(512)];            \
	TEST_SETUP(group)                                                      \
	{                                                                      \
		iobuffer_init((struct StIoBuffer*)SIMPTE_IO(group));           \
		io_putc(SIMPTE_IO(group), IO_EOF);                             \
		iobuffer_init((struct StIoBuffer*)SIMPTE_IO(group));           \
		SIMPTE_IO(STDOUT) = iofile_create(stdout);                     \
		SIMPTE_IO(STDERR) = iofile_create(stderr);                     \
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
		logger_detach(ERROR, SIMPTE_IO(group));                        \
		logger_detach(WARNING, SIMPTE_IO(group));                      \
		logger_detach(DEBUG, SIMPTE_IO(group));                        \
		logger_detach(INFO, SIMPTE_IO(group));                         \
		logger_detach(ERROR, SIMPTE_IO(STDERR));                       \
		logger_detach(WARNING, SIMPTE_IO(STDERR));                     \
		logger_detach(DEBUG, SIMPTE_IO(STDOUT));                       \
		logger_detach(INFO, SIMPTE_IO(STDOUT));                        \
		st_free(SIMPTE_IO(STDERR));                                    \
		SIMPTE_IO(STDERR) = NULL;                                      \
		st_free(SIMPTE_IO(STDOUT));                                    \
		SIMPTE_IO(STDOUT) = NULL;                                      \
		mem_cleanup();                                                 \
	}                                                                      \
	TEST_GROUP(group)

#define SIMPTE_GETTRACE(group, index) SimpteGettrace(SIMPTE_IO(group), index)

static inline void SimpteMain(int argc, const char** argv, void (*fn)(void))
{
	(void)argc;
	(void)argv;
	printf("Simpte test run 1 of 1\n");
	fn();
	printf("\n\n-----------------------\nOK\n");
}

static inline const char* SimpteGettrace(struct StIo* io, int index)
{
	static char out[256];

	iobuffer_init((struct StIoBuffer*)io);
	while (index--)
		io_fgets(out, sizeof(out), io);
	return io_fgets(out, sizeof(out), io);
}

#endif /* SIMPTE_H */
