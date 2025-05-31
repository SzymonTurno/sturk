#include "unittest.h"
#include "ub/os/mem.h"
#include "UB/str.h"
#include "pubsub.h"
#include <string.h>

TEST_GROUP(list);
TEST_SETUP(list) { return; }
TEST_TEAR_DOWN(list) { return; }

TEST(list, should_implement_lifo)
{
	struct CyStrlist* list = NULL;

	list = strlist_ins(NULL, "One");
	list = strlist_ins(list, "Two");
	list = strlist_ins(list, "Three");
	TEST_ASSERT_EQUAL_STRING("Three", strlist_rem(&list));
	TEST_ASSERT_EQUAL_STRING("Two", strlist_rem(&list));
	TEST_ASSERT_EQUAL_STRING("One", strlist_rem(&list));
	TEST_ASSERT_EQUAL(NULL, list);
}

TEST_GROUP(cirq);
TEST_SETUP(cirq) { return; }
TEST_TEAR_DOWN(cirq) { return; }

TEST(cirq, should_implement_fifo)
{
	struct CyStrq* q = NULL;

	q = strq_ins(NULL, "One");
	q = strq_ins(q, "Two");
	q = strq_ins(q, "Three");
	TEST_ASSERT_EQUAL_STRING("One", strq_rem(&q));
	TEST_ASSERT_EQUAL_STRING("Two", strq_rem(&q));
	TEST_ASSERT_EQUAL_STRING("Three", strq_rem(&q));
	TEST_ASSERT_EQUAL(NULL, q);
}

TEST_GROUP(broker);
TEST_SETUP(broker) { return; }
TEST_TEAR_DOWN(broker) { return; }

TEST(broker, should_support_single_thread_pubsub)
{
	const char* const expected[] = {
		LINE("[info] broadcast -3")
		LINE("[info] message: new = -3, old = 0")
		LINE("[info] message: new = 0, old = 0")
		LINE("[info] broadcast -13")
		LINE("[info] message: new = -13, old = -3")
		LINE("[info] message: new = 39, old = 0")
		LINE("[info] broadcast 7")
		LINE("[info] message: new = 7, old = -13")
		LINE("[info] message: new = -91, old = 39")
		LINE("[info] broadcast 1")
		LINE("[info] message: new = 1, old = 7")
		LINE("[info] message: new = 7, old = -91")
	};
	struct CyStrq* q = single_thread_pubsub();

	for (int i = 0; i < ARRAY_SIZE(expected); i++) {
		const char* tmp = strq_rem(&q);
		TEST_ASSERT_EQUAL_STRING(expected[i], tmp);
		ub_free((char*)tmp);
	}
	TEST_ASSERT_EQUAL(NULL, q);
}

TEST(broker, should_support_multi_thread_pubsub)
{
	struct CyStrbag* actual = multi_thread_pubsub();
	struct CyStrbag* tmp_a = actual;
	struct CyStrbag* expected = NULL;
	struct CyStrbag* tmp_e = NULL;

	expected = strbag_ins(expected, "[info] message: new = -3, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = -13, old = -3\n");
	expected = strbag_ins(expected, "[info] message: new = 1, old = 7\n");
	expected = strbag_ins(expected, "[info] message: new = 39, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 7, old = -13\n");
	expected = strbag_ins(expected, "[info] message: new = -91, old = 39\n");
	expected = strbag_ins(expected, "[info] message: new = 7, old = -91\n");
	tmp_e = expected;
	expected = dict_leftmost(expected);
	actual = dict_leftmost(actual);
	while (expected && actual) {
		TEST_ASSERT_EQUAL_STRING(dict_getk(expected),
			dict_getk(actual));
		TEST_ASSERT_EQUAL(strbag_count(expected),
			strbag_count(actual));
		expected = dict_next(expected);
		actual = dict_next(actual);
	}
	TEST_ASSERT_EQUAL(NULL, expected);
	TEST_ASSERT_EQUAL(NULL, actual);
	strbag_destroy(tmp_e);
	strbag_destroy(tmp_a);
}

TEST_GROUP(strbag);
TEST_SETUP(strbag) { return; }
TEST_TEAR_DOWN(strbag) { return; }

TEST(strbag, should_fill_up)
{
	struct CyStrbag* expected = NULL;

	expected = strbag_ins(expected, "[info] message: new = -3, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = -13, old = -3\n");
	expected = strbag_ins(expected, "[info] message: new = 1, old = 7\n");
	expected = strbag_ins(expected, "[info] message: new = 39, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[ino] message: new = 7, old = -13\n");
	expected = strbag_ins(expected, "[info] message: new = -91, old = 39\n");
	expected = strbag_ins(expected, "[info] message: new = 7, old = -91\n");
	strbag_destroy(expected);
}

static void run_all_tests(void)
{
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	if (POSIX_TESTS_ON)
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
	/* TODO: fix segfaults. */
	/* if (POSIX_TESTS_ON) */
	/* 	printf("\n"); */
	/* 	for (int i = 0; i < 1000; i++) { */
	/* 		printf("Ran %d times.\n", i); */
	/* 		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub); */
	/* 	} */
	/* RUN_TEST_CASE(strbag, should_fill_up); */
}

int main(int argc, const char **argv)
{
	log_attach(DEBUG, ub_stdout());
	log_attach(WARNING, ub_stderr());
	log_attach(ERROR, ub_stderr());
	UnityMain(argc, argv, run_all_tests);
	log_deinit();
	return 0;
}
