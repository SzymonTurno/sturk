#ifndef UNITTEST_H
#define UNITTEST_H

#include "unity.h"
#include "unity_fixture.h"
#include "UB/logger/log.h"
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define EXPECTED_LINE(str) str"\n",

static inline int assert_getline(UBfstream* stream, const char* str)
{
	int ret = 0;
	char* buff = malloc(256);

	ub_fgets(buff, 256, stream);
	ret = strcmp(buff, str);
	if (ret)
		fprintf(stderr, "Assertion failed: \"%s\" != \"%s\".\n", buff,
			str);
	free(buff);
	return ret;
}

#define TEST_ASSERT_LISTENER_ORDERED(listener, expected)                      \
	do {                                                                   \
		ub_fseekset((listener)->stream, 0);                            \
		for (size_t i = 0; i < ARRAY_SIZE(expected); i++)              \
			TEST_ASSERT_EQUAL(0, assert_getline((listener)->stream,\
				expected[i]));                                 \
		TEST_ASSERT_EQUAL(EOF, ub_fgetc((listener)->stream));          \
	} while (0)

#endif /* UNITTEST_H */
