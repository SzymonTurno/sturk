#include "pubsub.h"
#include "st/os/mem.h"
#include "sturk/arena.h"
#include "sturk/broker.h"
#include "sturk/cirq.h"
#include "sturk/dict.h"
#include "sturk/graph.h"
#include "sturk/io/bag.h"
#include "sturk/io/buffer.h"
#include "sturk/os/mutex.h"
#include "sturk/os/sem.h"
#include "sturk/pool.h"
#include "sturk/rbtree.h"
#include "sturk/str.h"
#include "sturk/waitq.h"
#include "unity.h"
#include "unity_fixture.h"

#include "extra/simpte.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define LINE(str) str "\n"

#if defined(_WIN32) || defined(WIN32)

#define JOIN_PATH(str1, str2) str1 "\\" str2

#else /* not defined: WIN32 */

#define JOIN_PATH(str1, str2) str1 "/" str2

#endif /* WIN32 */

#define MUTEX_FILE_PATH                                                        \
	JOIN_PATH(JOIN_PATH("src", "osal"), JOIN_PATH("mutex", "posix.c"))

#define RBTREE_FILE_PATH JOIN_PATH("src", JOIN_PATH("algo", "rbtree.c"))

#define BROKER_FILE_PATH JOIN_PATH("src", JOIN_PATH("broker", "broker.c"))

SIMPTE_BEGIN();

extern void run_vertegs_tests(void);
extern void run_broker_extra_tests(void);

SIMPTE_GROUP(common, "common");
SIMPTE_GROUP(list, "list");
SIMPTE_GROUP(cirq, "cirq");
SIMPTE_GROUP(rbnode, "rbnode");
SIMPTE_GROUP(dictnode, "dictnode");
SIMPTE_GROUP(strbag, "strbag");
SIMPTE_GROUP(mutex, "mutex");
SIMPTE_GROUP(semaphore, "semaphore");
SIMPTE_GROUP(waitq, "waitq");
SIMPTE_GROUP(pool, "pool");
SIMPTE_GROUP(arena, "arena");
SIMPTE_GROUP(io, "io");
SIMPTE_GROUP(channel, "channel");
SIMPTE_GROUP(subscriber, "subscriber");
SIMPTE_GROUP(broker, "broker");
SIMPTE_GROUP(logger, "logger");

TEST(common, should_destroy_null)
{
	pool_destroy(NULL);
	strbag_destroy(NULL);
	waitq_destroy(NULL);
	broker_destroy(NULL);
	subscriber_destroy(NULL);
	arena_free(NULL);
}

TEST(list, should_implement_lifo)
{
	struct StStrList* list = NULL;

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
	struct StStrQ* q = NULL;

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
	struct Vertegs* nbor[] = {left, right};

	TEST_ASSERT_EQUAL_PTR(
		left, (struct Vertegs*)rb_left((struct StRbNode*)nbor));
	TEST_ASSERT_EQUAL_PTR(
		right, (struct Vertegs*)rb_right((struct StRbNode*)nbor));
}

TEST(rbnode, should_link_as_leaf)
{
	struct StRbNode p = {0};
	struct StRbNode n = {0};

	memset(&n, 0xA, sizeof(n));
	TEST_ASSERT_EQUAL_PTR(&n, rb_link(&n, &p));
	/* Adding one ("+ 1") verifies that node is red. */
	TEST_ASSERT_EQUAL_INT(((intptr_t)&p) + 1, graph_datap(&n)->parcol);
	TEST_ASSERT_NULL(rb_left(&n));
	TEST_ASSERT_NULL(rb_right(&n));
}

TEST(rbnode, should_insert_and_balance)
{
	struct StRbNode c = {0};
	struct StRbNode a = {0};
	struct StRbNode b = {0};
	struct Vertegs** nbor = ((struct Vertegs*)&c)->nbor;

	/*
	 *    c
	 *   / \
	 *  a (nil)
	 */
	nbor[0] = graph_2vx(rb_link(&a, &c));

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
	nbor = ((struct Vertegs*)&a)->nbor;
	nbor[1] = graph_2vx(rb_link(&b, &a));

	/*
	 *        b
	 *       / \
	 *      a   c
	 */
	TEST_ASSERT_EQUAL_PTR(&b, rb_insrebal(&c, &b));
	nbor = ((struct Vertegs*)&b)->nbor;
	TEST_ASSERT_EQUAL_PTR(&a, nbor[0]);
	TEST_ASSERT_EQUAL_PTR(&c, nbor[1]);
}

TEST(rbnode, should_trace_not_supported_traversals)
{
	struct StRbNode node = {0};

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	rb_next(&node, BST_POSTORDER);
	TEST_ASSERT_EQUAL_STRING(
		RBTREE_FILE_PATH ":199: Not supported.\n",
		strstr(SIMPTE_GETTRACE(rbnode, 0), RBTREE_FILE_PATH ":"));
	rb_first(&node, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING(
		RBTREE_FILE_PATH ":160: Not supported.\n",
		strstr(SIMPTE_GETTRACE(rbnode, 1), RBTREE_FILE_PATH ":"));
}

TEST(dictnode, should_sort)
{
	struct StDictNode q = {.node = {0}, .str = "q"};
	struct StDictNode w = {.node = {0}, .str = "w"};
	struct StDictNode e = {.node = {0}, .str = "e"};
	struct StDictNode r = {.node = {0}, .str = "r"};
	struct StDictNode t = {.node = {0}, .str = "t"};
	struct StDictNode y = {.node = {0}, .str = "y"};
	struct StDictNode* root = dictnode_ins(NULL, &q);

	TEST_ASSERT_EQUAL_PTR(&q, root);
	root = dictnode_ins(root, &w);
	root = dictnode_ins(root, &e);
	root = dictnode_ins(root, &r);
	root = dictnode_ins(root, &t);
	root = dictnode_ins(root, &y);
	root = (struct StDictNode*)rb_first(
		(struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("e", root->str);
	root = (struct StDictNode*)rb_next((struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("q", root->str);
	root = (struct StDictNode*)rb_next((struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("r", root->str);
	root = (struct StDictNode*)rb_next((struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("t", root->str);
	root = (struct StDictNode*)rb_next((struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("w", root->str);
	root = (struct StDictNode*)rb_next((struct StRbNode*)root, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("y", root->str);
	TEST_ASSERT_NULL(rb_next((struct StRbNode*)root, BST_INORDER));
}

TEST(strbag, should_allow_many_entries)
{
	struct StStrBag* bag = NULL;
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
	struct StStrBag* bag = NULL;

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
	struct StStrBag* bag = NULL;

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
	bag = (struct StStrBag*)rb_next((struct StRbNode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	bag = (struct StStrBag*)rb_next((struct StRbNode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct StStrBag*)rb_next((struct StRbNode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = (struct StStrBag*)rb_next((struct StRbNode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = (struct StStrBag*)rb_next((struct StRbNode*)bag, BST_PREORDER);
	TEST_ASSERT_EQUAL_STRING("f", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_find_first)
{
	struct StStrBag* bag = NULL;
	struct StStrBag* tmp = NULL;

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
	tmp = (struct StStrBag*)rb_right((struct StRbNode*)bag);
	TEST_ASSERT_EQUAL_STRING("d", dict_getk(tmp));
	bag = (struct StStrBag*)rb_first((struct StRbNode*)tmp, BST_INORDER);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct StStrBag*)rb_first((struct StRbNode*)tmp, BST_POSTORDER);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(strbag, should_allow_negative_count)
{
	struct StStrBag* bag = strbag_rem(NULL, "");

	TEST_ASSERT_EQUAL_INT(-1, strbag_count(bag));
	strbag_destroy(bag);
}

TEST(mutex, should_not_block_on_trylock)
{
	StMutex* mutex = mutex_create(0);

	TEST_ASSERT_TRUE(mutex_trylock(mutex));
	TEST_ASSERT_FALSE(mutex_trylock(mutex));
	mutex_unlock(mutex);
	mutex_destroy(mutex);
}

TEST(mutex, should_lock_twice_if_recursive)
{
	StMutex* mutex = mutex_create(MUTEX_TYPE_RECURSIVE);

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

TEST(mutex, should_trace_double_lock_warning)
{
	StMutex* mut = mutex_create(0);

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	mutex_lock(mut);
	TEST_ASSERT_NULL(SIMPTE_GETTRACE(mutex, 0));
	mutex_lock(mut);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][sturk] Fake mutex does not support context "
		"switch.\n",
		SIMPTE_GETTRACE(mutex, 0));
	mutex_destroy(mut);
}

TEST(mutex, should_trace_double_unlock_warning)
{
	StMutex* mut = mutex_create(0);

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	TEST_ASSERT_NULL(SIMPTE_GETTRACE(mutex, 0));
	mutex_unlock(mut);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][sturk] Unlocking an already unlocked mutex.\n",
		SIMPTE_GETTRACE(mutex, 0));
	mutex_destroy(mut);
}

TEST(mutex, should_trace_not_supported_policy)
{
	logger_detach(WARNING, SIMPTE_IO(STDERR));
	(void)mutex_create(ST_MUTEX_BF(POLICY, 7));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH ":58: Not supported.\n",
		strstr(SIMPTE_GETTRACE(mutex, 0), MUTEX_FILE_PATH ":"));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH ":96: Mutex failure.\n",
		strstr(SIMPTE_GETTRACE(mutex, 1), MUTEX_FILE_PATH ":"));
}

TEST(mutex, should_trace_not_supported_type)
{
	logger_detach(WARNING, SIMPTE_IO(STDERR));
	(void)mutex_create(ST_MUTEX_BF(TYPE, 15));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH ":77: Not supported.\n",
		strstr(SIMPTE_GETTRACE(mutex, 0), MUTEX_FILE_PATH ":"));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH ":101: Mutex failure.\n",
		strstr(SIMPTE_GETTRACE(mutex, 1), MUTEX_FILE_PATH ":"));
}

TEST(semaphore, should_not_block_if_posted)
{
	StSem* sem = sem_create(0);

	sem_post(sem);
	sem_wait(sem);
	sem_destroy(sem);
}

TEST(semaphore, should_trace_fake_warning)
{
	StSem* sem = sem_create(0);

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	TEST_ASSERT_NULL(SIMPTE_GETTRACE(semaphore, 0));
	sem_wait(sem);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][sturk] Fake semaphore does not support context "
		"switch.\n",
		SIMPTE_GETTRACE(semaphore, 0));
	sem_destroy(sem);
}

TEST(waitq, should_not_block_after_insertion)
{
	struct Vertegs* nbor[] = {NULL, NULL};
	StWaitQ* waitq = waitq_create();

	waitq_ins(waitq, vx_4nbor(nbor));
	TEST_ASSERT_EQUAL_PTR(nbor, waitq_rem(waitq));
	waitq_destroy(waitq);
}

TEST(waitq, should_trace_dataloss)
{
	struct Vertegs* nbor[] = {NULL, NULL};
	StWaitQ* q = waitq_create();

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	waitq_ins(q, vx_4nbor(nbor));
	waitq_destroy(q);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][sturk] Data loss suspected.\n",
		SIMPTE_GETTRACE(waitq, 0));
}

TEST(pool, should_return_freed_pointer)
{
	StPool* pool = pool_create(1);
	void* mem = pool_alloc(pool);

	pool_free(pool, mem);
	TEST_ASSERT_EQUAL_PTR(mem, pool_alloc(pool));
	pool_free(pool, mem);
	pool_destroy(pool);
}

TEST(arena, should_allocate_aligned_memory)
{
	struct StArenaGroup g = {0};
	StArena* arena = arena_create(&g, malloc, free);
	void* buffs[8] = {0};

	for (int i = 0; i < ARRAY_SIZE(buffs); i++) {
		buffs[i] = ARENA_ALLOC(arena, 1 << i);
		TEST_ASSERT_EQUAL_INT(
			0, ((uintptr_t)buffs[i]) % sizeof(StAlign));
	}
	arena_destroy(arena);
	arena_cleanup(&g);
}

TEST(arena, should_allocate_independent_blocks)
{
	struct StArenaGroup g = {0};
	StArena* arena = arena_create(&g, malloc, free);
	void* buffs[15] = {0};

	for (int i = 0; i < ARRAY_SIZE(buffs); i++) {
		buffs[i] = ARENA_ALLOC(arena, i + 1);
		memset(buffs[i], i + 1, i + 1);
	}

	for (int i = 0; i < ARRAY_SIZE(buffs); i++)
		for (int j = 0; j < i + 1; j++)
			TEST_ASSERT_EQUAL_INT(i + 1, ((char*)buffs[i])[j]);
	arena_destroy(arena);
	arena_cleanup(&g);
}

TEST(arena, should_allocate_freed_memory)
{
	struct StArenaGroup g = {0};
	StArena* arena = arena_create(&g, malloc, free);
	void* buff = ARENA_ALLOC(arena, 64);

	memset(buff, 0xbe, 64);
	arena_free(arena);
	buff = ARENA_ALLOC(arena, 64);
	for (int i = 16; i < 48; i++)
		TEST_ASSERT_EQUAL_INT((char)0xbe, ((char*)buff)[i]);
	arena_destroy(arena);
	arena_cleanup(&g);
}

TEST(arena, should_return_null_for_null_arena)
{
	TEST_ASSERT_NULL(ARENA_ALLOC(NULL, 0));
}

TEST(arena, should_support_many_allocations)
{
	struct StArenaGroup g = {0};
	StArena* arena = arena_create(&g, malloc, free);
	struct StStrList* list = NULL;

	for (int i = 0; i < 16384; i++)
		list = strlist_ins(list, ARENA_ALLOC(arena, 128));

	while (list)
		strlist_rem(&list);
	arena_destroy(arena);
	arena_cleanup(&g);
}

TEST(io, should_calculate_iobuffer_length)
{
	const size_t remain = sizeof(StAlign) - sizeof(char*);

	TEST_ASSERT_EQUAL_INT(3, iobuffer_getlen(0));
	TEST_ASSERT_EQUAL_INT(3, iobuffer_getlen(remain));
	TEST_ASSERT_EQUAL_INT(4, iobuffer_getlen(remain + 1));
	TEST_ASSERT_EQUAL_INT(4, iobuffer_getlen(remain + sizeof(StAlign)));
	TEST_ASSERT_EQUAL_INT(5, iobuffer_getlen(remain + sizeof(StAlign) + 1));
}

TEST(io, should_write_to_memory_buffer)
{
	char out[256] = {0};
	StIoBuffer buff[iobuffer_getlen(sizeof(out))] = {0};
	StIo* io = io_init(buff);

	TEST_ASSERT_EQUAL_INT(sizeof(out) + 3 * sizeof(StAlign), sizeof(buff));
	io_print(io, "one ");
	io_print(io, "two \nthree");
	io_print(io, " %d;%u;%x", -3000L, 200, 10);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("one two \n", io_fgets(out, sizeof(out), io));
	TEST_ASSERT_EQUAL_STRING(
		"three -3000;200;a", io_fgets(out, sizeof(out), io));
	TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
	TEST_ASSERT_EQUAL_INT('t', out[0]);
	io = io_init(buff);
	io_print(io, "%01Alu", 14);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING(
		"00000000000000000014", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	io_print(io, "%01ald", -14L);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING(
		"00000000000000000-14", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	io_print(io, "%07lX", 11);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("000000B", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	io_print(io, "%c:%%:%s", 's', "string");
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("s:%:string", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	io_print(io, "%f%01b%", 3.14);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
	TEST_ASSERT_EQUAL_INT('s', out[0]);
}

TEST(io, should_read_from_file)
{
	const char* expected[] = {
		"Lorem ipsum dolor sit amet, consectetur adipiscing\n",
		"elit, sed do eiusmod tempor incididunt ut labore\n",
		"et dolore magna aliqua. Ut enim ad minim veniam,\n",
		"quis nostrud exercitation ullamco laboris nisi ut\n",
		"aliquip ex ea commodo consequat.\n"};
	char out[256] = {0};
	StIoBuffer buff[iobuffer_getlen(0)] = {0};
	StIo* io = io_init(buff);
	FILE* fp = fopen(LOREM_TXT, "r");

	io_setp(io, fp);
	io_setvp(io, SAMPLE_FILE_API);
	for (int i = 0; i < ARRAY_SIZE(expected); i++)
		TEST_ASSERT_EQUAL_STRING(
			expected[i], io_fgets(out, sizeof(out), io));
	TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
	TEST_ASSERT_EQUAL_INT(expected[4][0], out[0]);
	fclose(fp);
}

TEST(io, should_write_to_file)
{
	const char* filename = "sturk_tests_delete_me.txt";
	char out[256] = {0};
	StIoBuffer buff[iobuffer_getlen(0)] = {0};
	StIo* io = io_init(buff);
	FILE* fp = fopen(filename, "w");

	io_setp(io, fp);
	io_setvp(io, SAMPLE_FILE_API);
	io_print(io, "%d;%u;%x\n", -3000L, 200, 10);
	fclose(fp);
	fp = fopen(filename, "r");
	TEST_ASSERT_EQUAL_STRING("-3000;200;a\n", fgets(out, sizeof(out), fp));
	TEST_ASSERT_NULL(fgets(out, sizeof(out), fp));
	TEST_ASSERT_EQUAL_INT('-', out[0]);
	fclose(fp);
	remove(filename);
}

TEST(channel, should_access_its_topic)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StChannel* ch = broker_search(broker, "test");

	TEST_ASSERT_EQUAL_STRING("test", channel_gettopic(ch));
	broker_destroy(broker);
}

TEST(subscriber, should_receive_enqueued_message)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StSubscriber* sber = subscriber_create(broker);
	char** msg = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), 5);
	msg = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("|||||", *msg);
	broker_destroy(broker);
}

TEST(subscriber, should_trace_null_param)
{
	logger_detach(WARNING, SIMPTE_IO(STDERR));
	subscriber_unload(NULL);
	TEST_ASSERT_EQUAL_STRING(
		BROKER_FILE_PATH ":269: Null param.\n",
		strstr(SIMPTE_GETTRACE(subscriber, 0), BROKER_FILE_PATH ":"));
}

TEST(subscriber, should_unload)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StSubscriber* sber = subscriber_create(broker);
	char** msg = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), 2);
	msg = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("||", *msg);
	subscriber_unload(sber);
	publish(broker_search(broker, "test"), 5);
	TEST_ASSERT_EQUAL_STRING("|||||", *(char**)subscriber_await(sber));
	TEST_ASSERT_NOT_EQUAL_STRING("||", *msg);
	broker_destroy(broker);
}

TEST(subscriber, should_be_destroyed_independently)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StSubscriber* sber = subscriber_create(broker);

	subscriber_create(broker);
	subscriber_destroy(sber);
	broker_destroy(broker);
}

TEST(broker, should_allow_zero_subscribers)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StChannel* ch = broker_search(broker, "test");

	publish(ch, 123);
	broker_destroy(broker);
}

TEST(broker, should_allow_many_topics)
{
	StBroker* broker = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	for (int i = 0; i < 10; i++) {
		broker = broker_create(SAMPLE_MESSAGE_API);
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
	struct StStrQ* q = single_thread_pubsub();
	const char* tmp = NULL;

	for (int i = 0; i < ARRAY_SIZE(expected); i++) {
		tmp = strq_rem(&q);
		TEST_ASSERT_EQUAL_STRING(expected[i], tmp);
		st_free((char*)tmp);
	}
	TEST_ASSERT_NULL(q);
}

TEST(broker, should_support_multi_thread_pubsub)
{
	struct StStrBag* actual = multi_thread_pubsub();
	struct StStrBag* expected = NULL;

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

TEST(broker, should_trace_null_param)
{
	StSubscriber* tmp = calloc(sizeof(char), 256);

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	subscribe(tmp, NULL);
	TEST_ASSERT_EQUAL_STRING(
		BROKER_FILE_PATH ":308: Null param.\n",
		strstr(SIMPTE_GETTRACE(broker, 0), BROKER_FILE_PATH ":"));
	free(tmp);
}

TEST(logger, should_trace_debug)
{
	logger_detach(DEBUG, SIMPTE_IO(STDOUT));
	trace(DEBUG, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[debug] \n", SIMPTE_GETTRACE(logger, 0));
}

TEST(logger, should_trace_error)
{
	logger_detach(ERROR, SIMPTE_IO(STDERR));
	trace(ERROR, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[error] \n", SIMPTE_GETTRACE(logger, 0));
}

TEST(logger, should_ignore_detached_trace_levels)
{
	logger_detach(ERROR, SIMPTE_IO(STDERR));
	logger_detach(ERROR, SIMPTE_IO(logger));
	trace(ERROR, NULL, "");
}

static void run_extra_tests(void)
{
	run_vertegs_tests();
	if (THREADS_EN)
		run_broker_extra_tests();
}

static void run_basic_tests(void)
{
	RUN_TEST_CASE(common, should_destroy_null);
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(rbnode, should_return_left_and_right);
	RUN_TEST_CASE(rbnode, should_link_as_leaf);
	RUN_TEST_CASE(rbnode, should_insert_and_balance);
	RUN_TEST_CASE(rbnode, should_trace_not_supported_traversals);
	RUN_TEST_CASE(dictnode, should_sort);
	RUN_TEST_CASE(strbag, should_allow_many_entries);
	RUN_TEST_CASE(strbag, should_sort);
	RUN_TEST_CASE(strbag, should_allow_preorder_traversal);
	RUN_TEST_CASE(strbag, should_find_first);
	RUN_TEST_CASE(strbag, should_allow_negative_count);
	RUN_TEST_CASE(mutex, should_not_block_on_trylock);
	RUN_TEST_CASE(semaphore, should_not_block_if_posted);
	RUN_TEST_CASE(waitq, should_not_block_after_insertion);
	RUN_TEST_CASE(waitq, should_trace_dataloss);
	RUN_TEST_CASE(pool, should_return_freed_pointer);
	RUN_TEST_CASE(arena, should_allocate_aligned_memory);
	RUN_TEST_CASE(arena, should_allocate_independent_blocks);
	RUN_TEST_CASE(arena, should_allocate_freed_memory);
	RUN_TEST_CASE(arena, should_return_null_for_null_arena);
	RUN_TEST_CASE(arena, should_support_many_allocations);
	RUN_TEST_CASE(io, should_calculate_iobuffer_length);
	RUN_TEST_CASE(io, should_write_to_memory_buffer);
	RUN_TEST_CASE(io, should_read_from_file);
	RUN_TEST_CASE(io, should_write_to_file);
	RUN_TEST_CASE(channel, should_access_its_topic);
	RUN_TEST_CASE(subscriber, should_receive_enqueued_message);
	RUN_TEST_CASE(subscriber, should_trace_null_param);
	RUN_TEST_CASE(subscriber, should_unload);
	RUN_TEST_CASE(subscriber, should_be_destroyed_independently);
	RUN_TEST_CASE(broker, should_allow_zero_subscribers);
	RUN_TEST_CASE(broker, should_allow_many_topics);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	RUN_TEST_CASE(broker, should_trace_null_param);
	RUN_TEST_CASE(logger, should_trace_debug);
	RUN_TEST_CASE(logger, should_trace_error);
	RUN_TEST_CASE(logger, should_ignore_detached_trace_levels);
	if (THREADS_EN) {
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
		RUN_TEST_CASE(mutex, should_lock_twice_if_recursive);
		RUN_TEST_CASE(mutex, should_trace_not_supported_policy);
		RUN_TEST_CASE(mutex, should_trace_not_supported_type);
	} else {
		RUN_TEST_CASE(mutex, should_trace_double_lock_warning);
		RUN_TEST_CASE(mutex, should_trace_double_unlock_warning);
		RUN_TEST_CASE(semaphore, should_trace_fake_warning);
	}
}

int main(int argc, const char** argv)
{
	UnityMain(argc, argv, run_basic_tests);
	SimpteMain(argc, argv, run_extra_tests);
	return 0;
}
