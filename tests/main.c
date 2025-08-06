#include "cantil/broker.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
#include "cantil/graph.h"
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

#define SIMPLE_TEST_GROUP(group, name)                                         \
	TEST_SETUP(group)                                                      \
	{                                                                      \
		printf("\n");                                                  \
		logger_attach(INFO, cn_stdout());                              \
		trace(INFO, "ut", "Running test case for: %s.", name);         \
		logger_detach(INFO, cn_stdout());                              \
		return;                                                        \
	}                                                                      \
	TEST_TEAR_DOWN(group)                                                  \
	{                                                                      \
		logger_attach(INFO, cn_stdout());                              \
		trace(INFO, "ut", "Done.");                                    \
		logger_detach(INFO, cn_stdout());                              \
		logger_cleanup();                                              \
		return;                                                        \
	}                                                                      \
	TEST_GROUP(group)

static struct CnFstream* test_stream;

TEST_GROUP(logger);

TEST_SETUP(logger)
{
	test_stream = cn_fopen("logger_traces.tmp", "w+");

	logger_attach(INFO, cn_stdout());
	logger_attach(DEBUG, test_stream);
	logger_attach(WARNING, test_stream);
	logger_attach(ERROR, test_stream);
	printf("\n");
	trace(INFO, "ut", "Running test case for logger.");
}

TEST_TEAR_DOWN(logger)
{
	trace(INFO, "ut", "Done.");
	logger_cleanup();
	cn_fclose(test_stream);
	cn_remove("logger_traces.tmp");
}

static const char* gettrace(int index)
{
	static char buff[256];

	cn_fseekset(test_stream, 0);
	while (index--)
		cn_fgets(buff, sizeof(buff), test_stream);
	return cn_fgets(buff, sizeof(buff), test_stream);
}

SIMPLE_TEST_GROUP(common, "common");
SIMPLE_TEST_GROUP(list, "list");
SIMPLE_TEST_GROUP(cirq, "cirq");
SIMPLE_TEST_GROUP(rbnode, "rbnode");
SIMPLE_TEST_GROUP(strnode, "strnode");
SIMPLE_TEST_GROUP(strbag, "strbag");
SIMPLE_TEST_GROUP(mutex, "mutex");
SIMPLE_TEST_GROUP(sem, "sem");
SIMPLE_TEST_GROUP(waitq, "waitq");
SIMPLE_TEST_GROUP(binode, "binode");
SIMPLE_TEST_GROUP(pool, "pool");
SIMPLE_TEST_GROUP(subscriber, "subscriber");
SIMPLE_TEST_GROUP(broker, "broker");

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
	TEST_ASSERT_NULL(list);
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
	TEST_ASSERT_NULL(q);
}

TEST(rbnode, should_return_left_and_right)
{
	struct Vertegs left[1] = {0};
	struct Vertegs right[1] = {0};
	struct Vertegs* adjyl[] = {left, right};

	TEST_ASSERT_EQUAL_PTR(
		left, (struct Vertegs*)rb_left((struct CnRbnode*)adjyl));
	TEST_ASSERT_EQUAL_PTR(
		right, (struct Vertegs*)rb_right((struct CnRbnode*)adjyl));
}

TEST(rbnode, should_link_as_leaf)
{
	struct CnRbnode p = {0};
	struct CnRbnode n = {0};

	memset(&n, 0xA, sizeof(n));
	TEST_ASSERT_EQUAL_PTR(&n, rb_link(&n, &p));
	/* Adding one ("+ 1") verifies that node is red. */
	TEST_ASSERT_EQUAL_INT(((intptr_t)&p) + 1, graph_data(&n)->parcol);
	TEST_ASSERT_NULL(rb_left(&n));
	TEST_ASSERT_NULL(rb_right(&n));
}

TEST(rbnode, should_insert_and_balance)
{
	struct CnRbnode c = {0};
	struct CnRbnode a = {0};
	struct CnRbnode b = {0};
	struct Vertegs** adjyl = vx_2adjyl((struct Vertegs*)&c);

	/*
	 *    c
	 *   / \
	 *  a (nil)
	 */
	adjyl[0] = graph_2vx(rb_link(&a, &c));

	/*
	 *    c
	 *   / \
	 *  a (nil)
	 */
	TEST_ASSERT_EQUAL_PTR(&c, rb_insrebal(&c, &a));

	/*
	 *        c
	 *       / \
	 *      a (nil)
	 *     / \
	 *  (nil) b
	 */
	adjyl = vx_2adjyl((struct Vertegs*)&a);
	adjyl[1] = graph_2vx(rb_link(&b, &a));

	/*
	 *        b
	 *       / \
	 *      a   c
	 */
	TEST_ASSERT_EQUAL_PTR(&b, rb_insrebal(&c, &b));
	adjyl = vx_2adjyl((struct Vertegs*)&b);
	TEST_ASSERT_EQUAL_PTR(&a, adjyl[0]);
	TEST_ASSERT_EQUAL_PTR(&c, adjyl[1]);
}

TEST(strnode, should_sort)
{
	struct CnStrnode q = {.node = {0}, .str = "q"};
	struct CnStrnode w = {.node = {0}, .str = "w"};
	struct CnStrnode e = {.node = {0}, .str = "e"};
	struct CnStrnode r = {.node = {0}, .str = "r"};
	struct CnStrnode t = {.node = {0}, .str = "t"};
	struct CnStrnode y = {.node = {0}, .str = "y"};
	struct CnStrnode* root = strnode_ins(NULL, &q);

	TEST_ASSERT_EQUAL_PTR(&q, root);
	root = strnode_ins(root, &w);
	root = strnode_ins(root, &e);
	root = strnode_ins(root, &r);
	root = strnode_ins(root, &t);
	root = strnode_ins(root, &y);
	root = (struct CnStrnode*)rb_first((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("e", root->str);
	root = (struct CnStrnode*)rb_next((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("q", root->str);
	root = (struct CnStrnode*)rb_next((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("r", root->str);
	root = (struct CnStrnode*)rb_next((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("t", root->str);
	root = (struct CnStrnode*)rb_next((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("w", root->str);
	root = (struct CnStrnode*)rb_next((struct CnRbnode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("y", root->str);
	TEST_ASSERT_NULL(rb_next((struct CnRbnode*)root, BST_INORDER));
}

TEST(strbag, should_allow_many_entries)
{
	struct CnStrbag* bag = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	*((int*)str) = rand();
	bag = strbag_ins(bag, str);
	TEST_ASSERT_EQUAL_STRING(str, dict_getk(bag));
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
	TEST_ASSERT_NULL(dict_next(bag));
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
	bag = (struct CnStrbag*)rb_next((struct CnRbnode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next((struct CnRbnode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next((struct CnRbnode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next((struct CnRbnode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = (struct CnStrbag*)rb_next((struct CnRbnode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("f", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_find_first)
{
	struct CnStrbag* bag = NULL;
	struct CnStrbag* tmp = NULL;

	bag = strbag_ins(NULL, "c");
	bag = strbag_ins(bag, "a");
	bag = strbag_ins(bag, "d");
	bag = strbag_ins(bag, "b");
	/*
	 *     c
	 *    / \
	 *   /   \
	 *  a     d
	 *   \
	 *    b
	 */
	tmp = (struct CnStrbag*)rb_right((struct CnRbnode*)bag);
	TEST_ASSERT_EQUAL_STRING("d", dict_getk(tmp));
	bag = (struct CnStrbag*)rb_first((struct CnRbnode*)tmp, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct CnStrbag*)rb_first((struct CnRbnode*)tmp, BST_POSTORDER);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_allow_negative_count)
{
	struct CnStrbag* bag = strbag_rem(NULL, "");

	TEST_ASSERT_EQUAL_INT(-1, strbag_count(bag));
	strbag_destroy(bag);
}

TEST(mutex, should_not_block_on_trylock)
{
	CnMutex* mutex = mutex_create(0);

	TEST_ASSERT_TRUE(mutex_trylock(mutex));
	TEST_ASSERT_FALSE(mutex_trylock(mutex));
	mutex_unlock(mutex);
	mutex_destroy(mutex);
}

TEST(mutex, should_lock_twice_if_recursive)
{
	CnMutex* mutex = mutex_create(MUTEX_TYPE_RECURSIVE);

	mutex_lock(mutex);
	mutex_lock(mutex);
	TEST_ASSERT_TRUE(mutex_trylock(mutex));
	TEST_ASSERT_TRUE(mutex_trylock(mutex));
	mutex_unlock(mutex);
	mutex_unlock(mutex);
	mutex_unlock(mutex);
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
	struct Vertegs* adjyl[] = {NULL, NULL};
	CnWaitq* waitq = waitq_create();

	waitq_ins(waitq, (struct Vertegs*)adjyl);
	TEST_ASSERT_EQUAL_PTR(adjyl, waitq_rem(waitq));
	waitq_destroy(waitq);
}

TEST(binode, should_insert_at_any_position)
{
	GRAPH(struct Binode, 2, void*);

	const size_t next = 0;
	const size_t prev = 1;
	struct Binode n[5] = {0};

	/* -n0- */
	TEST_ASSERT_EQUAL_PTR(
		&n[0], cirq_ins((struct Binode*)NULL, &n[0], 255));
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[next], &n[0]);

	/* -n1--n0- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[0], &n[1], 0));
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[next], &n[1]);

	/* -n1--n0--n2- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[2], -1));
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[next], &n[1]);

	/* -n1--n3--n0--n- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[3], 1));
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[next], &n[3]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[3]))[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[3]))[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[prev], &n[3]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[next], &n[1]);

	/* -n1--n3--n0--n--n2- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[4], -2));
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[1]))[next], &n[3]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[3]))[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[3]))[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[prev], &n[3]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[0]))[next], &n[4]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[4]))[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[4]))[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[prev], &n[4]);
	TEST_ASSERT_EQUAL_PTR(vx_2adjyl(graph_2vx(&n[2]))[next], &n[1]);
}

TEST(pool, should_return_freed_pointer)
{
	CnPool* pool = pool_create(1);
	void* mem = pool_alloc(pool);

	pool_free(pool, mem);
	TEST_ASSERT_EQUAL_PTR(mem, pool_alloc(pool));
	pool_free(pool, mem);
	pool_destroy(pool);
}

TEST(subscriber, should_receive_enqueued_message)
{
	CnBroker* broker = broker_create(SAMPLE_LOAD_API);
	CnSubscriber* sber = subscriber_create(broker);
	CnChannel* ch = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%X", 0xF00D);
	TEST_ASSERT_NULL(channel_gettopic(ch));
	TEST_ASSERT_EQUAL_STRING("F00D", subscriber_await(sber, &ch));
	TEST_ASSERT_EQUAL_STRING("test", channel_gettopic(ch));
	broker_destroy(broker);
}

TEST(broker, should_allow_zero_subscribers)
{
	CnBroker* broker = broker_create(SAMPLE_LOAD_API);
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
		broker = broker_create(SAMPLE_LOAD_API);
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
	TEST_ASSERT_NULL(q);
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
		TEST_ASSERT_EQUAL_INT(
			strbag_count(expected), strbag_count(actual));
		if (!dict_next(expected) || !dict_next(actual))
			break;
		expected = dict_next(expected);
		actual = dict_next(actual);
	}
	TEST_ASSERT_NULL(dict_next(expected));
	TEST_ASSERT_NULL(dict_next(actual));
	strbag_destroy(expected);
	strbag_destroy(actual);
}

TEST(logger, should_trace_waitq_dataloss)
{
	struct Vertegs* adjyl[] = {NULL, NULL};
	CnWaitq* q = waitq_create();

	waitq_ins(q, (struct Vertegs*)adjyl);
	waitq_destroy(q);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Data loss suspected.\n", gettrace(0));
}

TEST(logger, should_trace_rbtree_not_supported)
{
	struct CnRbnode node = {0};

	rb_next(&node, BST_POSTORDER);
	TEST_ASSERT_EQUAL_STRING(
		"src/algo/rbtree.c:199: Not supported.\n",
		strstr(gettrace(0), "src/algo/rbtree.c:"));
	rb_first(&node, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING(
		"src/algo/rbtree.c:160: Not supported.\n",
		strstr(gettrace(1), "src/algo/rbtree.c:"));
}

TEST(logger, should_trace_debug)
{
	trace(DEBUG, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[debug] \n", gettrace(0));
}

TEST(logger, should_trace_error)
{
	trace(ERROR, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[error] \n", gettrace(0));
}

TEST(logger, should_do_nothing_if_not_initialized)
{
	logger_detach(INFO, cn_stdout());
	trace(INFO, NULL, "");
	logger_attach(INFO, cn_stdout());
}

TEST(logger, should_trace_null_params)
{
	CnSubscriber* tmp = calloc(sizeof(char), 256);

	subscribe(tmp, NULL);
	TEST_ASSERT_EQUAL_STRING(
		"src/broker/broker.c:309: Null param.\n",
		strstr(gettrace(0), "src/broker/broker.c:"));
	free(tmp);
}

TEST(logger, should_trace_mutex_double_lock_warning)
{
	CnMutex* mutex = mutex_create(0);

	mutex_lock(mutex);
	TEST_ASSERT_NULL(gettrace(0));
	mutex_lock(mutex);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Fake mutex does not support context "
		"switch.\n",
		gettrace(0));
	mutex_destroy(mutex);
}

TEST(logger, should_trace_mutex_double_unlock_warning)
{
	CnMutex* mutex = mutex_create(0);

	TEST_ASSERT_NULL(gettrace(0));
	mutex_unlock(mutex);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Unlocking an already unlocked mutex.\n",
		gettrace(0));
	mutex_destroy(mutex);
}

TEST(logger, should_trace_fake_semaphore_warning)
{
	CnSem* sem = sem_create(0);

	TEST_ASSERT_NULL(gettrace(0));
	sem_wait(sem);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][cantil] Fake semaphore does not support context "
		"switch.\n",
		gettrace(0));
	sem_destroy(sem);
}

TEST(logger, should_trace_not_supported_mutex_policy)
{
	CnMutex* mutex = mutex_create(CN_MUTEX_BF(POLICY, 7));

	TEST_ASSERT_EQUAL_STRING(
		"src/osal/posix/mutex.c:58: Not supported.\n",
		strstr(gettrace(0), "src/osal/posix/mutex.c:"));
	TEST_ASSERT_EQUAL_STRING(
		"src/osal/posix/mutex.c:96: Mutex failure.\n",
		strstr(gettrace(1), "src/osal/posix/mutex.c:"));
}

TEST(logger, should_trace_not_supported_mutex_type)
{
	CnMutex* mutex = mutex_create(CN_MUTEX_BF(TYPE, 15));

	TEST_ASSERT_EQUAL_STRING(
		"src/osal/posix/mutex.c:77: Not supported.\n",
		strstr(gettrace(0), "src/osal/posix/mutex.c:"));
	TEST_ASSERT_EQUAL_STRING(
		"src/osal/posix/mutex.c:101: Mutex failure.\n",
		strstr(gettrace(1), "src/osal/posix/mutex.c:"));
}

static void run_all_tests(void)
{
	RUN_TEST_CASE(common, should_destroy_null);
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(rbnode, should_return_left_and_right);
	RUN_TEST_CASE(rbnode, should_link_as_leaf);
	RUN_TEST_CASE(rbnode, should_insert_and_balance);
	RUN_TEST_CASE(strnode, should_sort);
	RUN_TEST_CASE(strbag, should_allow_many_entries);
	RUN_TEST_CASE(strbag, should_sort);
	RUN_TEST_CASE(strbag, should_allow_preorder_traversal);
	RUN_TEST_CASE(strbag, should_find_first);
	RUN_TEST_CASE(strbag, should_allow_negative_count);
	RUN_TEST_CASE(mutex, should_not_block_on_trylock);
	RUN_TEST_CASE(sem, should_not_block_if_posted);
	RUN_TEST_CASE(waitq, should_not_block_after_insertion);
	RUN_TEST_CASE(binode, should_insert_at_any_position);
	RUN_TEST_CASE(pool, should_return_freed_pointer)
	RUN_TEST_CASE(subscriber, should_receive_enqueued_message);
	RUN_TEST_CASE(broker, should_allow_zero_subscribers);
	RUN_TEST_CASE(broker, should_allow_many_topics);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	RUN_TEST_CASE(logger, should_trace_waitq_dataloss);
	RUN_TEST_CASE(logger, should_trace_rbtree_not_supported);
	RUN_TEST_CASE(logger, should_trace_debug);
	RUN_TEST_CASE(logger, should_trace_error);
	RUN_TEST_CASE(logger, should_do_nothing_if_not_initialized);
	RUN_TEST_CASE(logger, should_trace_null_params)
	if (THREADS_EN) {
		RUN_TEST_CASE(mutex, should_lock_twice_if_recursive);
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
		RUN_TEST_CASE(logger, should_trace_not_supported_mutex_policy);
		RUN_TEST_CASE(logger, should_trace_not_supported_mutex_type);
	} else {
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
