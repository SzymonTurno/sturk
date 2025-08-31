#ifndef SIMPTE_H
#define SIMPTE_H

#include "sturk/logger/trace.h"
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
		printf(".\nRunning test case %d for: %s.\n",                   \
		       ++simpte_##group##_counter, simpte_##group##_label);    \
		simpte_##group##_##case();                                     \
		printf("Done\n");                                              \
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

#ifdef TEST_GROUP

#define SIMPTE_GROUP(group, label)                                             \
	struct StFstream* simpte_stream_##group;                               \
	TEST_SETUP(group)                                                      \
	{                                                                      \
		simpte_stream_##group =                                        \
			st_fopen("test_traces_" label ".tmp", "w+");           \
                                                                               \
		logger_attach(INFO, st_stdout());                              \
		logger_attach(DEBUG, st_stdout());                             \
		logger_attach(WARNING, st_stderr());                           \
		logger_attach(ERROR, st_stderr());                             \
		printf("\n");                                                  \
		trace(INFO, "ut", "Running test case for: %s.", label);        \
		logger_attach(INFO, simpte_stream_##group);                    \
		logger_attach(DEBUG, simpte_stream_##group);                   \
		logger_attach(WARNING, simpte_stream_##group);                 \
		logger_attach(ERROR, simpte_stream_##group);                   \
	}                                                                      \
	TEST_TEAR_DOWN(group)                                                  \
	{                                                                      \
		trace(INFO, "ut", "Done.");                                    \
		logger_cleanup();                                              \
		st_fclose(simpte_stream_##group);                              \
		st_remove("test_traces_" label ".tmp");                        \
	}                                                                      \
	TEST_GROUP(group)

#else /* not defined: TEST_GROUP */

#define SIMPTE_GROUP(group, label)                                             \
	int simpte_##group##_counter;                                          \
	const char* simpte_##group##_label = label

#endif /* TEST_GROUP */

#define SIMPTE_DETACH(group, lvl) logger_detach((lvl), simpte_stream_##group)

#define SIMPTE_GETTRACE(group, index)                                          \
	SimpteGettrace(simpte_stream_##group, index)

static inline void SimpteMain(int argc, const char** argv, void (*fn)(void))
{
	(void)argc;
	(void)argv;
	printf("Simpte test run 1 of 1\n");
	fn();
	printf("\n\n-----------------------\nOK\n");
}

static inline const char* SimpteGettrace(struct StFstream* stream, int index)
{
	static char buff[256];

	st_fseekset(stream, 0);
	while (index--)
		st_fgets(buff, sizeof(buff), stream);
	return st_fgets(buff, sizeof(buff), stream);
}

#endif /* SIMPTE_H */
