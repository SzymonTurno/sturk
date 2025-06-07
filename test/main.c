#include "unity.h"
#include "unity_fixture.h"
#include "cn/os/mem.h"
#include "cantil/str.h"
#include "cantil/dict.h"
#include "cantil/logger/log.h"
#include "pubsub.h"
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define LINE(str) str"\n"

TEST_GROUP(list);
TEST_SETUP(list) { return; }
TEST_TEAR_DOWN(list) { return; }

TEST(list, should_implement_lifo)
{
	struct CnStrlist* list = NULL;

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
	struct CnStrq* q = NULL;

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
		LINE("[info] broadcast -3"),
		LINE("[info] message: new = -3, old = 0"),
		LINE("[info] message: new = 0, old = 0"),
		LINE("[info] broadcast -13"),
		LINE("[info] message: new = -13, old = -3"),
		LINE("[info] message: new = 39, old = 0"),
		LINE("[info] broadcast 7"),
		LINE("[info] message: new = 7, old = -13"),
		LINE("[info] message: new = -91, old = 39"),
		LINE("[info] broadcast 1"),
		LINE("[info] message: new = 1, old = 7"),
		LINE("[info] message: new = 7, old = -91")
	};
	struct CnStrq* q = single_thread_pubsub();

	for (int i = 0; i < ARRAY_SIZE(expected); i++) {
		const char* tmp = strq_rem(&q);
		TEST_ASSERT_EQUAL_STRING(expected[i], tmp);
		cn_free((char*)tmp);
	}
	TEST_ASSERT_EQUAL(NULL, q);
}

TEST(broker, should_support_multi_thread_pubsub)
{
	struct CnStrbag* actual = multi_thread_pubsub();
	struct CnStrbag* tmp_a = actual;
	struct CnStrbag* expected = NULL;
	struct CnStrbag* tmp_e = NULL;

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

TEST(strbag, should_handle_all_rb_tree_insertion_cases)
{
	struct CnStrbag* bag = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	for (int i = 0; i < 10; i++) {
		for (int i = 0; i < 1000; i++) {
			*((int*)str) = rand();
			bag = strbag_ins(bag, str);
		}
		strbag_destroy(bag);
		bag = NULL;
	}
}

static void run_all_tests(void)
{
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(strbag, should_handle_all_rb_tree_insertion_cases);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	if (POSIX_TESTS_ON)
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
}

int main(int argc, const char **argv)
{
	log_attach(DEBUG, cn_stdout());
	log_attach(WARNING, cn_stderr());
	log_attach(ERROR, cn_stderr());
	UnityMain(argc, argv, run_all_tests);
	log_cleanup();
	return 0;
}
