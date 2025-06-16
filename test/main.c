#include "cantil/broker.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mutex.h"
#include "cantil/os/sem.h"
#include "cantil/pool.h"
#include "cantil/rbtree.h"
#include "cantil/str.h"
#include "cantil/waitq.h"
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

static struct CnFstream* test_stream;

TEST_GROUP(logger);

TEST_SETUP(logger)
{
	test_stream = cn_fopen("logger_traces.tmp", "w+");

	logger_attach(DEBUG, test_stream);
	logger_attach(WARNING, test_stream);
	logger_attach(ERROR, test_stream);
}

TEST_TEAR_DOWN(logger)
{
	logger_cleanup();
	cn_fclose(test_stream);
	cn_remove("logger_traces.tmp");
}

static const char* gettrace(void)
{
	static char buff[256];

	cn_fseekset(test_stream, 0);
	return cn_fgets(buff, sizeof(buff), test_stream);
}

SIMPLE_TEST_GROUP(common);
SIMPLE_TEST_GROUP(list);
SIMPLE_TEST_GROUP(cirq);
SIMPLE_TEST_GROUP(strbag);
SIMPLE_TEST_GROUP(mutex);
SIMPLE_TEST_GROUP(sem);
SIMPLE_TEST_GROUP(waitq);
SIMPLE_TEST_GROUP(binode);
SIMPLE_TEST_GROUP(broker);
SIMPLE_TEST_GROUP(subscriber);

TEST(common, should_destroy_null)
{
	pool_destroy(NULL);
	strbag_destroy(NULL);
	waitq_destroy(NULL);
	broker_destroy(NULL);
	subscriber_destroy(NULL);
}

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

TEST(strbag, should_allow_many_entries)
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
	bag = dict_first(bag);
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
	TEST_ASSERT_EQUAL(NULL, dict_next(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_allow_preorder_traversal)
{
	struct CnStrbag* bag = NULL;

	bag = strbag_ins(NULL, "d");
	bag = strbag_ins(bag, "b");
	bag = strbag_ins(bag, "e");
	bag = strbag_ins(bag, "a");
	bag = strbag_ins(bag, "c");
	bag = strbag_ins(bag, "f");
	/*
	 *      d
	 *     / \
	 *    b   e
	 *   / \   \
	 *  a   c   f
	 */
	TEST_ASSERT_EQUAL_STRING("d", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next(
		(struct CnRbnode*)bag, BST_TRAV_PREORDER);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next(
		(struct CnRbnode*)bag, BST_TRAV_PREORDER);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next(
		(struct CnRbnode*)bag, BST_TRAV_PREORDER);
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next(
		(struct CnRbnode*)bag, BST_TRAV_PREORDER);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next(
		(struct CnRbnode*)bag, BST_TRAV_PREORDER);
	TEST_ASSERT_EQUAL_STRING("f", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_find_smallest)
{
	struct CnStrbag* bag = NULL;

	bag = strbag_ins(NULL, "b");
	bag = strbag_ins(bag, "a");
	bag = strbag_ins(bag, "c");
	/*
	 *    b
	 *   / \
	 *  a   c
	 */
	bag = (struct CnStrbag*)((struct CnRbnode*)bag)->right;
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = (struct CnStrbag*)rb_smallest((struct CnRbnode*)bag);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_allow_negative_count)
{
	struct CnStrbag* bag = strbag_rem(NULL, "");

	TEST_ASSERT_EQUAL(-1, strbag_count(bag));
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

TEST(waitq, should_not_block_after_insertion)
{
	struct CnBinode node = {0};
	CnWaitq* waitq = waitq_create();

	waitq_ins(waitq, &node);
	TEST_ASSERT_EQUAL(&node, waitq_rem(waitq));
	waitq_destroy(waitq);
}

TEST(binode, should_insert_at_any_position)
{
	struct CnBinode n[5] = {0};

	/* -n0- */
	TEST_ASSERT_EQUAL(&n[0], binode_ins(NULL, &n[0], 255));
	TEST_ASSERT_EQUAL(n[0].prev, &n[0]);
	TEST_ASSERT_EQUAL(n[0].next, &n[0]);

	/* -n1--n0- */
	TEST_ASSERT_EQUAL(&n[1], binode_ins(&n[0], &n[1], 0));
	TEST_ASSERT_EQUAL(n[1].prev, &n[0]);
	TEST_ASSERT_EQUAL(n[1].next, &n[0]);
	TEST_ASSERT_EQUAL(n[0].prev, &n[1]);
	TEST_ASSERT_EQUAL(n[0].next, &n[1]);

	/* -n1--n0--n2- */
	TEST_ASSERT_EQUAL(&n[1], binode_ins(&n[1], &n[2], -1));
	TEST_ASSERT_EQUAL(n[1].prev, &n[2]);
	TEST_ASSERT_EQUAL(n[1].next, &n[0]);
	TEST_ASSERT_EQUAL(n[0].prev, &n[1]);
	TEST_ASSERT_EQUAL(n[0].next, &n[2]);
	TEST_ASSERT_EQUAL(n[2].prev, &n[0]);
	TEST_ASSERT_EQUAL(n[2].next, &n[1]);

	/* -n1--n3--n0--n2- */
	TEST_ASSERT_EQUAL(&n[1], binode_ins(&n[1], &n[3], 1));
	TEST_ASSERT_EQUAL(n[1].prev, &n[2]);
	TEST_ASSERT_EQUAL(n[1].next, &n[3]);
	TEST_ASSERT_EQUAL(n[3].prev, &n[1]);
	TEST_ASSERT_EQUAL(n[3].next, &n[0]);
	TEST_ASSERT_EQUAL(n[0].prev, &n[3]);
	TEST_ASSERT_EQUAL(n[0].next, &n[2]);
	TEST_ASSERT_EQUAL(n[2].prev, &n[0]);
	TEST_ASSERT_EQUAL(n[2].next, &n[1]);

	/* -n1--n3--n0--n4--n2- */
	TEST_ASSERT_EQUAL(&n[1], binode_ins(&n[1], &n[4], -2));
	TEST_ASSERT_EQUAL(n[1].prev, &n[2]);
	TEST_ASSERT_EQUAL(n[1].next, &n[3]);
	TEST_ASSERT_EQUAL(n[3].prev, &n[1]);
	TEST_ASSERT_EQUAL(n[3].next, &n[0]);
	TEST_ASSERT_EQUAL(n[0].prev, &n[3]);
	TEST_ASSERT_EQUAL(n[0].next, &n[4]);
	TEST_ASSERT_EQUAL(n[4].prev, &n[0]);
	TEST_ASSERT_EQUAL(n[4].next, &n[2]);
	TEST_ASSERT_EQUAL(n[2].prev, &n[4]);
	TEST_ASSERT_EQUAL(n[2].next, &n[1]);
}

TEST(broker, should_allow_zero_subscribers)
{
	CnBroker* broker = broker_create(DEFAULT_LOAD_VP);
	CnChannel* ch = broker_search(broker, "test");

	publish(ch, "%d", 123);
	broker_destroy(broker);
}

TEST(broker, should_allow_many_topics)
{
	CnBroker* broker = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	for (int i = 0; i < 10; i++) {
		broker = broker_create(DEFAULT_LOAD_VP);
		for (int i = 0; i < 1000; i++) {
			*((int*)str) = rand();
			broker_search(broker, str);
		}
		broker_destroy(broker);
		broker = NULL;
	}
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
	const char* tmp = NULL;

	for (int i = 0; i < ARRAY_SIZE(expected); i++) {
		tmp = strq_rem(&q);
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
	expected = dict_first(expected);
	actual = dict_first(actual);
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

TEST(subscriber, should_not_wait_after_publishing)
{
	CnBroker* broker = broker_create(DEFAULT_LOAD_VP);
	CnSubscriber* sber = subscriber_create(broker);
	CnChannel* ch = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%d", 321);
	TEST_ASSERT_EQUAL_STRING("321", *(char**)subscriber_await(sber, &ch));
	TEST_ASSERT_EQUAL_STRING("test", get_topic(ch));
	broker_destroy(broker);
}

TEST(logger, should_trace_waitq_dataloss)
{
	CnWaitq* q = waitq_create();
	struct CnBinode node = {0};

	waitq_ins(q, &node);
	waitq_destroy(q);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Data loss suspected.\n", gettrace());
}

TEST(logger, should_trace_rbtree_postorder_not_supported)
{
	struct CnRbnode node = {0};

	rb_next(&node, BST_TRAV_POSTORDER);
	TEST_ASSERT_EQUAL_STRING(
		"[warning] src/algo/rbtree.c:123: Not supported.\n",
		gettrace());
}

TEST(logger, should_trace_debug)
{
	trace(DEBUG, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[debug] \n", gettrace());
}

TEST(logger, should_trace_error)
{
	trace(ERROR, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[error] \n", gettrace());
}

TEST(logger, should_do_nothing_if_not_initialized)
{
	trace(INFO, NULL, "");
}

TEST(logger, should_trace_mutex_double_lock_warning)
{
	CnMutex* mutex = mutex_create(0);

	mutex_lock(mutex);
	TEST_ASSERT_EQUAL_STRING(NULL, gettrace());
	mutex_lock(mutex);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Fake mutex does not support context "
		"switch.\n",
		gettrace());
	mutex_destroy(mutex);
}

TEST(logger, should_trace_mutex_double_unlock_warning)
{
	CnMutex* mutex = mutex_create(0);

	TEST_ASSERT_EQUAL_STRING(NULL, gettrace());
	mutex_unlock(mutex);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Unlocking an already unlocked mutex.\n",
		gettrace());
	mutex_destroy(mutex);
}

TEST(logger, should_trace_fake_semaphore_warning)
{
	CnSem* sem = sem_create(0);

	TEST_ASSERT_EQUAL_STRING(NULL, gettrace());
	sem_wait(sem);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Fake semaphore does not support context "
		"switch.\n",
		gettrace());
	sem_destroy(sem);
}

static void run_all_tests(void)
{
	logger_attach(DEBUG, cn_stdout());
	logger_attach(WARNING, cn_stderr());
	logger_attach(ERROR, cn_stderr());
	RUN_TEST_CASE(common, should_destroy_null);
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(strbag, should_allow_many_entries);
	RUN_TEST_CASE(strbag, should_sort);
	RUN_TEST_CASE(strbag, should_allow_preorder_traversal);
	RUN_TEST_CASE(strbag, should_find_smallest);
	RUN_TEST_CASE(strbag, should_allow_negative_count);
	RUN_TEST_CASE(mutex, should_not_block_on_trylock);
	RUN_TEST_CASE(sem, should_not_block_if_posted);
	RUN_TEST_CASE(waitq, should_not_block_after_insertion);
	RUN_TEST_CASE(binode, should_insert_at_any_position);
	RUN_TEST_CASE(broker, should_allow_zero_subscribers);
	RUN_TEST_CASE(broker, should_allow_many_topics);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	if (MULTITHREADING_EN)
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
	RUN_TEST_CASE(subscriber, should_not_wait_after_publishing);
	logger_cleanup();
	RUN_TEST_CASE(logger, should_trace_waitq_dataloss);
	RUN_TEST_CASE(logger, should_trace_rbtree_postorder_not_supported);
	RUN_TEST_CASE(logger, should_trace_debug);
	RUN_TEST_CASE(logger, should_trace_error);
	RUN_TEST_CASE(logger, should_do_nothing_if_not_initialized);
	if (!MULTITHREADING_EN) {
		RUN_TEST_CASE(logger, should_trace_mutex_double_lock_warning);
		RUN_TEST_CASE(logger, should_trace_mutex_double_unlock_warning);
		RUN_TEST_CASE(logger, should_trace_fake_semaphore_warning);
	}
}

int main(int argc, const char** argv)
{
	UnityMain(argc, argv, run_all_tests);
	return 0;
}
