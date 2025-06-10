#include "cantil/dict.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mutex.h"
#include "cantil/os/sem.h"
#include "cantil/str.h"
#include "cn/os/mem.h"
#include "pubsub.h"
#include "unity.h"
#include "unity_fixture.h"
#include <string.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define LINE(str) str "\n"

#define SIMPLE_TEST_GROUP(group)                                               \
	TEST_SETUP(group)                                                      \
	{                                                                      \
		return;                                                        \
	}                                                                      \
	TEST_TEAR_DOWN(group)                                                  \
	{                                                                      \
		return;                                                        \
	}                                                                      \
	TEST_GROUP(group)

SIMPLE_TEST_GROUP(list);
SIMPLE_TEST_GROUP(cirq);
SIMPLE_TEST_GROUP(strbag);
SIMPLE_TEST_GROUP(mutex);
SIMPLE_TEST_GROUP(sem);
SIMPLE_TEST_GROUP(broker);

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

TEST(strbag, should_sort)
{
	struct CnStrbag* bag = NULL;

	bag = strbag_ins(NULL, "q");
	bag = strbag_ins(bag, "w");
	bag = strbag_ins(bag, "e");
	bag = strbag_ins(bag, "r");
	bag = strbag_ins(bag, "t");
	bag = strbag_ins(bag, "y");
	bag = dict_leftmost(bag);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("q", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("r", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("t", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("w", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("y", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(mutex, should_not_block_on_trylock)
{
	CnMutex* mutex = mutex_create(0);

	TEST_ASSERT_EQUAL(true, mutex_trylock(mutex));
	TEST_ASSERT_EQUAL(false, mutex_trylock(mutex));
	mutex_unlock(mutex);
	mutex_destroy(mutex);
}

TEST(sem, should_not_block_if_posted)
{
	CnSem* sem = sem_create(0);

	sem_post(sem);
	sem_wait(sem);
	sem_destroy(sem);
}

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
		LINE("[info] message: new = 7, old = -91")};
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
	struct CnStrbag* expected = NULL;

	expected = strbag_ins(expected, "[info] message: new = -3, old = 0\n");
	expected =
		strbag_ins(expected, "[info] message: new = -13, old = -3\n");
	expected = strbag_ins(expected, "[info] message: new = 1, old = 7\n");
	expected = strbag_ins(expected, "[info] message: new = 39, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 0, old = 0\n");
	expected = strbag_ins(expected, "[info] message: new = 7, old = -13\n");
	expected =
		strbag_ins(expected, "[info] message: new = -91, old = 39\n");
	expected = strbag_ins(expected, "[info] message: new = 7, old = -91\n");
	expected = dict_leftmost(expected);
	actual = dict_leftmost(actual);
	for (;;) {
		TEST_ASSERT_EQUAL_STRING(
			dict_getk(expected), dict_getk(actual));
		TEST_ASSERT_EQUAL(strbag_count(expected), strbag_count(actual));
		if (!dict_next(expected) || !dict_next(actual))
			break;
		expected = dict_next(expected);
		actual = dict_next(actual);
	}
	TEST_ASSERT_EQUAL(NULL, dict_next(expected));
	TEST_ASSERT_EQUAL(NULL, dict_next(actual));
	strbag_destroy(expected);
	strbag_destroy(actual);
}

static void run_all_tests(void)
{
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(strbag, should_handle_all_rb_tree_insertion_cases);
	RUN_TEST_CASE(strbag, should_sort);
	RUN_TEST_CASE(mutex, should_not_block_on_trylock);
	RUN_TEST_CASE(sem, should_not_block_if_posted);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	if (POSIX_TESTS_ON)
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
}

int main(int argc, const char** argv)
{
	logger_attach(DEBUG, cn_stdout());
	logger_attach(WARNING, cn_stderr());
	logger_attach(ERROR, cn_stderr());
	UnityMain(argc, argv, run_all_tests);
	logger_cleanup();
	return 0;
}
