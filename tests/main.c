#include "sample.h"
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
#include "sturk/os/sys.h"
#include "sturk/pool.h"
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

#define MUTEX_FILE_PATH(impl)                                                  \
	JOIN_PATH(JOIN_PATH("src", "osal"), JOIN_PATH("mutex", impl ".c"))

#define SEM_FILE_PATH(impl)                                                    \
	JOIN_PATH(JOIN_PATH("src", "osal"), JOIN_PATH("sem", impl ".c"))

#define RBTREE_FILE_PATH JOIN_PATH("src", JOIN_PATH("algo", "rbtree.c"))

#define BROKER_FILE_PATH JOIN_PATH("src", JOIN_PATH("broker", "broker.c"))

const struct StMemVt STURK_MEM_API[] = {{.alloc_cb = malloc, .free_cb = free}};

extern void run_vertegs_tests(void);
extern void run_traffic_tests(void);

SIMPTE_IO_DCL(debug, 512);

TEST_SETUP(basis)
{
	printf("\n");
}
TEST_TEAR_DOWN(basis)
{
}
TEST_GROUP(basis);

TEST_SETUP(osal)
{
	SIMPTE_IO_INIT(debug);
	io_putc(SIMPTE_IO(debug), IO_EOF);
	SIMPTE_IO_INIT(debug);
	printf("\n");
}
TEST_TEAR_DOWN(osal)
{
	mem_cleanup();
}
TEST_GROUP(osal);

SIMPTE_BEGIN();
SIMPTE_GROUP(logger);
SIMPTE_GROUP(algo);
SIMPTE_GROUP(broker);
SIMPTE_GROUP(common);

#if VX_DEBUGGING
void vx_dprint(const char* text, const char* file, int line)
{
	io_print(SIMPTE_IO(debug), "%s:%d: %s\n", file, line, text);
}
#endif /* VX_DEBUGGING */

TEST(basis, should_destroy_null)
{
	arena_free(NULL);
}

TEST(basis, should_allocate_aligned_memory_from_arena)
{
	struct StArenaGc gc = {0};
	StArena* arena = arena_create(&gc, STURK_MEM_API);
	void* buffs[8] = {0};

	for (int i = 0; i < ARRAY_SIZE(buffs); i++) {
		buffs[i] = ARENA_ALLOC(arena, 1 << i);
		TEST_ASSERT_EQUAL_INT(
			0, ((uintptr_t)buffs[i]) % sizeof(StAlign));
	}
	arena_destroy(arena);
	arena_cleanup(&gc);
}

TEST(basis, should_allocate_nonoverlapping_blocks_from_arena)
{
	struct StArenaGc gc = {0};
	StArena* arena = arena_create(&gc, STURK_MEM_API);
	void* buffs[15] = {0};

	for (int i = 0; i < ARRAY_SIZE(buffs); i++) {
		buffs[i] = ARENA_ALLOC(arena, i + 1);
		memset(buffs[i], i + 1, i + 1);
	}

	for (int i = 0; i < ARRAY_SIZE(buffs); i++)
		for (int j = 0; j < i + 1; j++)
			TEST_ASSERT_EQUAL_INT(i + 1, ((char*)buffs[i])[j]);
	arena_destroy(arena);
	arena_cleanup(&gc);
}

TEST(basis, should_allocate_freed_memory_from_arena)
{
	struct StArenaGc gc = {0};
	StArena* arena = arena_create(&gc, STURK_MEM_API);
	void* buff = ARENA_ALLOC(arena, 64);

	memset(buff, 0xbe, 64);
	arena_free(arena);
	buff = ARENA_ALLOC(arena, 64);
	for (int i = 16; i < 48; i++)
		TEST_ASSERT_EQUAL_INT((char)0xbe, ((char*)buff)[i]);
	arena_destroy(arena);
	arena_cleanup(&gc);
}

TEST(basis, should_alloc_null_from_null_arena)
{
	TEST_ASSERT_NULL(ARENA_ALLOC(NULL, 0));
}

TEST(basis, should_calculate_iobuffer_length)
{
	const size_t remain = sizeof(StAlign) - sizeof(char*);

	TEST_ASSERT_EQUAL_INT(3, iobuffer_calclen(0));
	TEST_ASSERT_EQUAL_INT(3, iobuffer_calclen(remain));
	TEST_ASSERT_EQUAL_INT(4, iobuffer_calclen(remain + 1));
	TEST_ASSERT_EQUAL_INT(4, iobuffer_calclen(remain + sizeof(StAlign)));
	TEST_ASSERT_EQUAL_INT(
		5, iobuffer_calclen(remain + sizeof(StAlign) + 1));
}

TEST(basis, should_write_to_memory_buffer)
{
	char out[16] = {0};

	/* Allocate sixteen bytes plus metadata for the memory buffer. */
	StIoBuffer buff[iobuffer_calclen(sizeof(out))] = {0};

	/* Initialize the memory buffer. */
	StIo* io = io_init(buff);

	/* Print "deadbeef" to the memory buffer. */
	io_print(io, "%x", 0xDEADBEEF);

	/* Put EOF character in the memory buffer. */
	io_putc(io, IO_EOF);

	/* Initialize the memory buffer again to read from the beginning. */
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("deadbeef", io_fgets(out, sizeof(out), io));
}

TEST(basis, should_mimic_sprintf_with_io)
{
	char out[256] = {0};
	StIoBuffer buff[iobuffer_calclen(sizeof(out))] = {0};
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
	TEST_ASSERT_EQUAL_INT(20, io_print(io, "%020lu", 14));
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING(
		"00000000000000000014", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	TEST_ASSERT_EQUAL_INT(20, io_print(io, "%020ld", -14L));
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING(
		"-0000000000000000014", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	TEST_ASSERT_EQUAL_INT(7, io_print(io, "%07lX", 11));
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("000000B", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	TEST_ASSERT_EQUAL_INT(10, io_print(io, "%c:%%:%s", 's', "string"));
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_EQUAL_STRING("s:%:string", io_fgets(out, sizeof(out), io));
	io = io_init(buff);
	TEST_ASSERT_EQUAL_INT(0, io_print(io, "%f%01b%", 3.14));
	io_putc(io, IO_EOF);
	io = io_init(buff);
	TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
	TEST_ASSERT_EQUAL_INT('s', out[0]);
}

TEST(basis, should_read_from_file)
{
	const char* expected[] = {
		"Lorem ipsum dolor sit amet, consectetur adipiscing\n",
		"elit, sed do eiusmod tempor incididunt ut labore\n",
		"et dolore magna aliqua. Ut enim ad minim veniam,\n",
		"quis nostrud exercitation ullamco laboris nisi ut\n",
		"aliquip ex ea commodo consequat.\n"};
	char out[256] = {0};
	StIoBuffer buff[iobuffer_calclen(0)] = {0};
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

TEST(basis, should_write_to_file)
{
	const char* filename = "sturk_tests_delete_me.txt";
	char out[256] = {0};
	StIoBuffer buff[iobuffer_calclen(0)] = {0};
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

TEST(basis, should_support_many_allocations_from_arena)
{
	struct StArenaGc gc = {0};
	StArena* arena = arena_create(&gc, STURK_MEM_API);

	for (int i = 0; i < 2048; i++)
		TEST_ASSERT_NOT_NULL(ARENA_ALLOC(arena, 128));

	arena_destroy(arena);
	arena_cleanup(&gc);
}

static void* test_malloc(size_t size)
{
	static int allocated;
	static int data[1024];

	if (allocated)
		return NULL;
	allocated = 1;
	return data;
}

static void test_free(void* ptr)
{
	(void)ptr;
}

static const struct StMemVt TEST_MEM_API[] = {
	{.alloc_cb = test_malloc, .free_cb = test_free}};

TEST(basis, should_return_null_from_arena)
{
	struct StArenaGc gc = {0};
	StArena* arena = arena_create(&gc, TEST_MEM_API);

	TEST_ASSERT_NULL(ARENA_ALLOC(arena, 1));
	arena_destroy(arena);
	arena_cleanup(&gc);
}

TEST_GROUP_RUNNER(basis)
{
	printf("BASIS TESTS\n");
	RUN_TEST_CASE(basis, should_destroy_null);
	RUN_TEST_CASE(basis, should_allocate_aligned_memory_from_arena);
	RUN_TEST_CASE(basis, should_allocate_nonoverlapping_blocks_from_arena);
	RUN_TEST_CASE(basis, should_allocate_freed_memory_from_arena);
	RUN_TEST_CASE(basis, should_alloc_null_from_null_arena);
	RUN_TEST_CASE(basis, should_calculate_iobuffer_length);
	RUN_TEST_CASE(basis, should_write_to_memory_buffer);
	RUN_TEST_CASE(basis, should_mimic_sprintf_with_io);
	RUN_TEST_CASE(basis, should_read_from_file);
	RUN_TEST_CASE(basis, should_write_to_file);
	RUN_TEST_CASE(basis, should_support_many_allocations_from_arena);
	RUN_TEST_CASE(basis, should_return_null_from_arena);
}

TEST(osal, should_not_block_on_mutex_trylock)
{
	StMutex* mutex = mutex_create(0);

	TEST_ASSERT_TRUE(mutex_trylock(mutex));
	TEST_ASSERT_FALSE(mutex_trylock(mutex));
	mutex_unlock(mutex);
	mutex_destroy(mutex);
}

TEST(osal, should_not_block_if_sem_posted)
{
	StSem* sem = sem_create(0);

	sem_post(sem);
	sem_wait(sem);
	sem_destroy(sem);
}

TEST(osal, should_write_to_string)
{
	char out[256] = {0};

	TEST_ASSERT_EQUAL_INT(4, strprint(out, "one "));
	TEST_ASSERT_EQUAL_STRING("one ", out);
	TEST_ASSERT_EQUAL_INT(10, strprint(out, "two \nthree"));
	TEST_ASSERT_EQUAL_STRING("two \nthree", out);
	TEST_ASSERT_EQUAL_INT(12, strprint(out, " %d;%u;%x", -3000L, 200, 10));
	TEST_ASSERT_EQUAL_STRING(" -3000;200;a", out);
	TEST_ASSERT_EQUAL_INT(12, strprint(out, "%012lu", 14));
	TEST_ASSERT_EQUAL_STRING("000000000014", out);
	TEST_ASSERT_EQUAL_INT(12, strprint(out, "%012ld", -14L));
	TEST_ASSERT_EQUAL_STRING("-00000000014", out);
	TEST_ASSERT_EQUAL_INT(7, strprint(out, "%07lX", 11));
	TEST_ASSERT_EQUAL_STRING("000000B", out);
	TEST_ASSERT_EQUAL_INT(10, strprint(out, "%c:%%:%s", 's', "string"));
	TEST_ASSERT_EQUAL_STRING("s:%:string", out);
}

TEST(osal, should_lock_mutex_twice_if_recursive)
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

TEST(osal, should_trace_mutex_not_supported_policy)
{
	(void)mutex_create(ST_MUTEX_BF(POLICY, 7));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH("posix") ":61: Not supported.\n",
		strstr(SIMPTE_GETTRACE(debug, 0),
	               MUTEX_FILE_PATH("posix") ":"));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH("posix") ":102: Mutex failure.\n",
		strstr(SIMPTE_GETTRACE(debug, 1),
	               MUTEX_FILE_PATH("posix") ":"));
}

TEST(osal, should_trace_mutex_not_supported_type)
{
	(void)mutex_create(ST_MUTEX_BF(TYPE, 15));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH("posix") ":83: Not supported.\n",
		strstr(SIMPTE_GETTRACE(debug, 0),
	               MUTEX_FILE_PATH("posix") ":"));
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH("posix") ":107: Mutex failure.\n",
		strstr(SIMPTE_GETTRACE(debug, 1),
	               MUTEX_FILE_PATH("posix") ":"));
}

TEST(osal, should_trace_mutex_double_lock_warning)
{
	StMutex* mut = mutex_create(0);

	mutex_lock(mut);
	TEST_ASSERT_NULL(SIMPTE_GETTRACE(debug, 0));
	mutex_lock(mut);
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH("none") ":65: Fake mutex does not support "
					"context switch.\n",
		strstr(SIMPTE_GETTRACE(debug, 0), MUTEX_FILE_PATH("none") ":"));
	mutex_destroy(mut);
}

TEST(osal, should_trace_mutex_double_unlock_warning)
{
	StMutex* mut = mutex_create(0);

	TEST_ASSERT_NULL(SIMPTE_GETTRACE(debug, 0));
	mutex_unlock(mut);
	TEST_ASSERT_EQUAL_STRING(
		MUTEX_FILE_PATH(
			"none") ":90: Unlocking an already unlocked mutex.\n",
		strstr(SIMPTE_GETTRACE(debug, 0), MUTEX_FILE_PATH("none") ":"));
	mutex_destroy(mut);
}

TEST(osal, should_trace_sem_fake_warning)
{
	StSem* sem = sem_create(0);

	TEST_ASSERT_NULL(SIMPTE_GETTRACE(debug, 0));
	sem_wait(sem);
	TEST_ASSERT_EQUAL_STRING(
		SEM_FILE_PATH("none") ":64: Fake semaphore does not support "
				      "context switch.\n",
		strstr(SIMPTE_GETTRACE(debug, 0), SEM_FILE_PATH("none") ":"));
	sem_destroy(sem);
}

TEST(osal, should_reach_mem_limit)
{
	for (int i = 0; i < 7; i++)
		TEST_ASSERT_NOT_NULL(st_alloc(8 * 512));
	TEST_ASSERT_NULL(st_alloc(8 * 512));
}

TEST_GROUP_RUNNER(osal)
{
	printf("OSAL TESTS\n");
	RUN_TEST_CASE(osal, should_not_block_on_mutex_trylock);
	RUN_TEST_CASE(osal, should_not_block_if_sem_posted);
	RUN_TEST_CASE(osal, should_write_to_string);
	if (MULTITHREADING)
		RUN_TEST_CASE(osal, should_lock_mutex_twice_if_recursive);

	if (MULTITHREADING && VX_DEBUGGING) {
		RUN_TEST_CASE(osal, should_trace_mutex_not_supported_policy);
		RUN_TEST_CASE(osal, should_trace_mutex_not_supported_type);
	}

	if (!MULTITHREADING && VX_DEBUGGING) {
		RUN_TEST_CASE(osal, should_trace_mutex_double_lock_warning);
		RUN_TEST_CASE(osal, should_trace_mutex_double_unlock_warning);
		RUN_TEST_CASE(osal, should_trace_sem_fake_warning);
	}

	if (MEM_LIMITED)
		RUN_TEST_CASE(osal, should_reach_mem_limit);
}

TEST(logger, should_remove_io_when_destroying_iobag)
{
	char out[16] = {0};
	StIoBuffer buff[iobuffer_calclen(sizeof(out))] = {0};
	StIo* io = io_init(buff);
	StIoBag* bag = iobag_create();

	iobag_ins(bag, io);
	iobag_destroy(bag);
}

TEST(logger, should_trace_debug)
{
	logger_detach(DEBUG, SIMPTE_IO(STDOUT));
	trace(DEBUG, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[debug] \n", SIMPTE_GETTRACE(logger, 0));
	logger_attach(DEBUG, SIMPTE_IO(STDOUT));
}

TEST(logger, should_trace_error)
{
	logger_detach(ERROR, SIMPTE_IO(STDERR));
	trace(ERROR, NULL, "");
	TEST_ASSERT_EQUAL_STRING("[error] \n", SIMPTE_GETTRACE(logger, 0));
	logger_attach(ERROR, SIMPTE_IO(STDERR));
}

TEST(logger, should_ignore_detached_trace_levels)
{
	logger_detach(ERROR, SIMPTE_IO(STDERR));
	logger_detach(ERROR, SIMPTE_IO(logger));
	trace(ERROR, NULL, "");
	logger_attach(ERROR, SIMPTE_IO(logger));
	logger_attach(ERROR, SIMPTE_IO(STDERR));
}

TEST_GROUP_RUNNER(logger)
{
	printf("LOGGER TESTS\n");
	RUN_TEST_CASE(logger, should_remove_io_when_destroying_iobag);
	RUN_TEST_CASE(logger, should_trace_debug);
	RUN_TEST_CASE(logger, should_trace_error);
	RUN_TEST_CASE(logger, should_ignore_detached_trace_levels);
}

TEST(algo, should_destroy_null)
{
	pool_destroy(NULL);
	strbag_destroy(NULL);
	waitq_destroy(NULL);
}

TEST(algo, should_implement_list_as_lifo)
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

TEST(algo, should_implement_cirq_as_fifo)
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

TEST(algo, should_return_left_and_right_child_of_rbnode)
{
	struct Vertegs left[1] = {0};
	struct Vertegs right[1] = {0};
	struct Vertegs* nbor[] = {left, right};

	TEST_ASSERT_EQUAL_PTR(left, vx_4nbor(nbor)->nbor[VX_RB_LEFT]);
	TEST_ASSERT_EQUAL_PTR(right, vx_4nbor(nbor)->nbor[VX_RB_RIGHT]);
}

TEST(algo, should_link_rbnode_as_leaf)
{
	struct Vertegs* p[VX_RB_N_EDGES] = {0};
	struct Vertegs* n[VX_RB_N_EDGES] = {0};

	memset(n, 0xA, sizeof(n));
	TEST_ASSERT_EQUAL_PTR(vx_4nbor(n), vx_linkrb(vx_4nbor(n), vx_4nbor(p)));
	/* Adding one ("+ 1") verifies that node is red. */
	TEST_ASSERT_EQUAL_INT(
		((intptr_t)vx_4nbor(p)) + 1, (intptr_t)n[VX_RB_PARCOL]);
	TEST_ASSERT_NULL(vx_4nbor(n)->nbor[VX_RB_LEFT]);
	TEST_ASSERT_NULL(vx_4nbor(n)->nbor[VX_RB_RIGHT]);
}

TEST(algo, should_insert_in_rbtree_and_balance)
{
	struct Vertegs* c[VX_RB_N_EDGES] = {0};
	struct Vertegs* a[VX_RB_N_EDGES] = {0};
	struct Vertegs* b[VX_RB_N_EDGES] = {0};
	struct Vertegs** nbor = c;

	/*
	 *    c
	 *   / \
	 *  a (nil)
	 */
	nbor[0] = vx_linkrb(vx_4nbor(a), vx_4nbor(c));

	/*
	 *    c
	 *   / \
	 *  a (nil)
	 */
	TEST_ASSERT_EQUAL_PTR(
		vx_4nbor(c), vx_insrebalrb(vx_4nbor(c), vx_4nbor(a)));

	/*
	 *        c
	 *       / \
	 *      a (nil)
	 *     / \
	 *  (nil) b
	 */
	nbor = a;
	nbor[1] = vx_linkrb(vx_4nbor(b), vx_4nbor(a));

	/*
	 *        b
	 *       / \
	 *      a   c
	 */
	TEST_ASSERT_EQUAL_PTR(
		vx_4nbor(b), vx_insrebalrb(vx_4nbor(c), vx_4nbor(b)));
	nbor = b;
	TEST_ASSERT_EQUAL_PTR(vx_4nbor(a), nbor[0]);
	TEST_ASSERT_EQUAL_PTR(vx_4nbor(c), nbor[1]);
}

TEST(algo, should_sort_with_dict_node)
{
	struct StDictNode q = {0};
	struct StDictNode w = {0};
	struct StDictNode e = {0};
	struct StDictNode r = {0};
	struct StDictNode t = {0};
	struct StDictNode y = {0};
	struct StDictNode* root = NULL;

	*graph_datap(&q) = "q";
	*graph_datap(&w) = "w";
	*graph_datap(&e) = "e";
	*graph_datap(&r) = "r";
	*graph_datap(&t) = "t";
	*graph_datap(&y) = "y";
	root = dictnode_ins(NULL, &q);
	TEST_ASSERT_EQUAL_PTR(&q, root);
	root = dictnode_ins(root, &w);
	root = dictnode_ins(root, &e);
	root = dictnode_ins(root, &r);
	root = dictnode_ins(root, &t);
	root = dictnode_ins(root, &y);
	root = (struct StDictNode*)vx_inorderfirst((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("e", *graph_datap(root));
	root = (struct StDictNode*)vx_inordernext((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("q", *graph_datap(root));
	root = (struct StDictNode*)vx_inordernext((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("r", *graph_datap(root));
	root = (struct StDictNode*)vx_inordernext((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("t", *graph_datap(root));
	root = (struct StDictNode*)vx_inordernext((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("w", *graph_datap(root));
	root = (struct StDictNode*)vx_inordernext((struct Vertegs*)root);
	TEST_ASSERT_EQUAL_STRING("y", *graph_datap(root));
	TEST_ASSERT_NULL(vx_inordernext((struct Vertegs*)root));
}

TEST(algo, should_find_first_in_dict)
{
	struct StStrBag* bag = NULL;
	struct StStrBag* first = NULL;

	/*
	 *        c
	 *       / \
	 *      a   d
	 *     / \
	 *  (nil) b
	 */
	bag = strbag_ins(NULL, "c");
	bag = strbag_ins(bag, "a");
	bag = strbag_ins(bag, "d");
	bag = strbag_ins(bag, "b");
	bag = dict_first(bag);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(algo, should_allow_many_entries_in_strbag)
{
	struct StStrBag* bag = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	*(int*)str = rand();
	bag = strbag_ins(bag, str);
	TEST_ASSERT_EQUAL_STRING(str, dict_getk(bag));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 1000; j++) {
			*(int*)str = rand();
			bag = strbag_ins(bag, str);
		}
		strbag_destroy(bag);
		bag = NULL;
	}
}

TEST(algo, should_sort_with_strbag)
{
	struct StStrBag* bag = NULL;

	bag = strbag_ins(NULL, "q");
	bag = strbag_ins(bag, "w");
	bag = strbag_ins(bag, "e");
	bag = strbag_ins(bag, "r");
	bag = strbag_ins(bag, "t");
	bag = strbag_ins(bag, "y");
	bag = strbag_ins(bag, "u");
	bag = strbag_ins(bag, "i");
	bag = strbag_ins(bag, "o");
	bag = strbag_ins(bag, "p");
	bag = strbag_ins(bag, "a");
	bag = strbag_ins(bag, "s");
	bag = strbag_ins(bag, "d");
	bag = strbag_ins(bag, "f");
	bag = strbag_ins(bag, "g");
	bag = strbag_ins(bag, "h");
	bag = strbag_ins(bag, "j");
	bag = strbag_ins(bag, "k");
	bag = strbag_ins(bag, "l");
	bag = strbag_ins(bag, "z");
	bag = strbag_ins(bag, "x");
	bag = strbag_ins(bag, "c");
	bag = strbag_ins(bag, "v");
	bag = strbag_ins(bag, "b");
	bag = strbag_ins(bag, "n");
	bag = strbag_ins(bag, "m");
	bag = dict_first(bag);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("d", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("f", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("g", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("h", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("i", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("j", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("k", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("l", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("m", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("n", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("o", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("p", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("q", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("r", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("s", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("t", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("u", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("v", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("w", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("x", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("y", dict_getk(bag));
	bag = dict_next(bag);
	TEST_ASSERT_EQUAL_STRING("z", dict_getk(bag));
	TEST_ASSERT_NULL(dict_next(bag));
	strbag_destroy(bag);
}

TEST(algo, should_allow_preorder_traversal_with_strbag)
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
	bag = (struct StStrBag*)vx_preordernext((struct Vertegs*)bag);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	bag = (struct StStrBag*)vx_preordernext((struct Vertegs*)bag);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct StStrBag*)vx_preordernext((struct Vertegs*)bag);
	TEST_ASSERT_EQUAL_STRING("c", dict_getk(bag));
	bag = (struct StStrBag*)vx_preordernext((struct Vertegs*)bag);
	TEST_ASSERT_EQUAL_STRING("e", dict_getk(bag));
	bag = (struct StStrBag*)vx_preordernext((struct Vertegs*)bag);
	TEST_ASSERT_EQUAL_STRING("f", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(algo, should_find_first_in_strbag)
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
	tmp = (struct StStrBag*)((struct Vertegs*)bag)->nbor[VX_RB_RIGHT];
	TEST_ASSERT_EQUAL_STRING("d", dict_getk(tmp));
	bag = (struct StStrBag*)vx_inorderfirst((struct Vertegs*)tmp);
	TEST_ASSERT_EQUAL_STRING("a", dict_getk(bag));
	bag = (struct StStrBag*)vx_postorderfirst((struct Vertegs*)tmp);
	TEST_ASSERT_EQUAL_STRING("b", dict_getk(bag));
	strbag_destroy(bag);
}

TEST(algo, should_allow_negative_count_in_strbag)
{
	struct StStrBag* bag = strbag_rem(NULL, "");

	TEST_ASSERT_EQUAL_INT(-1, strbag_count(bag));
	strbag_destroy(bag);
}

TEST(algo, should_not_block_after_insertion_in_waitq)
{
	struct Vertegs* nbor[] = {NULL, NULL};
	StWaitQ* waitq = waitq_create();

	waitq_ins(waitq, vx_4nbor(nbor));
	TEST_ASSERT_EQUAL_PTR(nbor, waitq_rem(waitq));
	waitq_destroy(waitq);
}

TEST(algo, should_trace_waitq_dataloss)
{
	struct Vertegs* nbor[] = {NULL, NULL};
	StWaitQ* q = waitq_create();

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	waitq_ins(q, vx_4nbor(nbor));
	waitq_destroy(q);
	TEST_ASSERT_EQUAL_STRING(
		"[warning][sturk] Data loss suspected.\n",
		SIMPTE_GETTRACE(algo, 0));
	logger_attach(WARNING, SIMPTE_IO(STDERR));
}

TEST(algo, should_return_freed_pointer_from_pool)
{
	StPool* pool = pool_create(1);
	void* mem = POOL_ALLOC(pool);

	pool_free(pool, mem);
	TEST_ASSERT_EQUAL_PTR(mem, POOL_ALLOC(pool));
	pool_free(pool, mem);
	pool_destroy(pool);
}

TEST_GROUP_RUNNER(algo)
{
	printf("ALGO TESTS\n");
	RUN_TEST_CASE(algo, should_destroy_null);
	RUN_TEST_CASE(algo, should_implement_list_as_lifo);
	RUN_TEST_CASE(algo, should_implement_cirq_as_fifo);
	RUN_TEST_CASE(algo, should_return_left_and_right_child_of_rbnode);
	RUN_TEST_CASE(algo, should_link_rbnode_as_leaf);
	RUN_TEST_CASE(algo, should_insert_in_rbtree_and_balance);
	RUN_TEST_CASE(algo, should_sort_with_dict_node);
	RUN_TEST_CASE(algo, should_find_first_in_dict);
	if (!MEM_LIMITED)
		RUN_TEST_CASE(algo, should_allow_many_entries_in_strbag);
	RUN_TEST_CASE(algo, should_sort_with_strbag);
	RUN_TEST_CASE(algo, should_allow_preorder_traversal_with_strbag);
	RUN_TEST_CASE(algo, should_find_first_in_strbag);
	RUN_TEST_CASE(algo, should_allow_negative_count_in_strbag);
	RUN_TEST_CASE(algo, should_not_block_after_insertion_in_waitq);
	RUN_TEST_CASE(algo, should_trace_waitq_dataloss);
	RUN_TEST_CASE(algo, should_return_freed_pointer_from_pool);
}

TEST(broker, should_destroy_null)
{
	broker_destroy(NULL);
	subscriber_destroy(NULL);
}

TEST(broker, should_get_topic_from_channel)
{
	StBroker* broker = broker_create(0);
	StChannel* ch = broker_search(broker, "test");

	TEST_ASSERT_EQUAL_STRING("test", channel_gettopic(ch));
	broker_destroy(broker);
}

TEST(broker, should_return_null_payload_for_null_channel)
{
	logger_detach(WARNING, SIMPTE_IO(STDERR));
	TEST_ASSERT_NULL(message_alloc(NULL).payload);
	logger_attach(WARNING, SIMPTE_IO(STDERR));
}

static void test_freepayload(struct StMessage msg)
{
	st_free(*(char**)msg.payload);
}

TEST(broker, should_forward_message)
{
	StBroker* broker = broker_create(sizeof(char*));
	StSubscriber* sber = subscriber_create(broker);
	StChannel* ch = broker_search(broker, "test.topic");
	struct StMessage msg = {0};

	/* Subscribe */
	subscribe(sber, "test.topic");
	TEST_ASSERT_NULL(subscriber_poll(sber).payload);

	/* Publish */
	broker_adjust(broker, 1);
	msg = message_alloc(ch);
	*(char**)msg.payload = st_strdup("test contents");
	message_setcb(msg, test_freepayload);
	publish(&msg);
	TEST_ASSERT_NULL(msg.payload);

	/* Receive */
	msg = subscriber_poll(sber);
	TEST_ASSERT_EQUAL_STRING("test contents", *(char**)msg.payload);

	/* Cleanup */
	broker_destroy(broker);
}

TEST(broker, should_trace_null_subscriber)
{
	logger_detach(WARNING, SIMPTE_IO(STDERR));
	subscriber_unload(NULL);
	TEST_ASSERT_EQUAL_STRING(
		BROKER_FILE_PATH ":441: Null param.\n",
		strstr(SIMPTE_GETTRACE(broker, 0), BROKER_FILE_PATH ":"));
	logger_attach(WARNING, SIMPTE_IO(STDERR));
}

TEST(broker, should_unload_subscriber)
{
	StBroker* broker = broker_create(sizeof(char*));
	StSubscriber* sber = subscriber_create(broker);
	StChannel* ch = broker_search(broker, "test.topic");
	struct StMessage msg = {0};
	struct StMessage alice = {0};

	subscribe(sber, "test.topic");
	broker_adjust(broker, 1);
	msg = message_alloc(ch);
	*(char**)msg.payload = st_strdup("Alice");
	message_setcb(msg, test_freepayload);
	publish(&msg);
	alice = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("Alice", *(char**)alice.payload);

	/* Return "Alice" message to the pool. */
	subscriber_unload(sber);

	/* Reusing memory previously used for "Alice" message. */
	msg = message_alloc(ch);

	message_setcb(msg, test_freepayload);
	*(char**)msg.payload = st_strdup("Bob");
	publish(&msg);
	TEST_ASSERT_EQUAL_STRING(
		"Bob", *(char**)subscriber_await(sber).payload);

	/* As expected: "Alice" message, previously received by the subcriber,
	 * has been corrupted. */
	TEST_ASSERT_NOT_EQUAL_STRING("Alice", *(char**)alice.payload);

	broker_destroy(broker);
}

TEST(broker, should_destroy_any_subscriber)
{
	StBroker* broker = broker_create(0);
	StSubscriber* sber = subscriber_create(broker);

	subscriber_create(broker);
	subscriber_destroy(sber);
	broker_destroy(broker);
}

TEST(broker, should_allow_zero_subscribers)
{
	StBroker* broker = broker_create(0);
	StChannel* ch = broker_search(broker, "test");
	struct StMessage msg = {0};

	broker_adjust(broker, 1);
	msg = message_alloc(ch);
	publish(&msg);
	broker_destroy(broker);
}

TEST(broker, should_allow_many_topics)
{
	StBroker* broker = NULL;
	char str[sizeof(int) + 1] = {0};

	srand(1);
	for (int i = 0; i < 10; i++) {
		broker = broker_create(0);
		for (int j = 0; j < 1000; j++) {
			*((int*)str) = rand();
			broker_search(broker, str);
		}
		broker_destroy(broker);
		broker = NULL;
	}
}

TEST(broker, should_trace_null_broker)
{
	StSubscriber* tmp = calloc(sizeof(char), 256);

	logger_detach(WARNING, SIMPTE_IO(STDERR));
	subscribe(tmp, NULL);
	TEST_ASSERT_EQUAL_STRING(
		BROKER_FILE_PATH ":469: Null param.\n",
		strstr(SIMPTE_GETTRACE(broker, 0), BROKER_FILE_PATH ":"));
	free(tmp);
	logger_attach(WARNING, SIMPTE_IO(STDERR));
}

TEST(broker, should_alloc_message)
{
	StBroker* broker = broker_create(0);
	StChannel* ch = broker_search(broker, "test");
	struct StMessage msg = message_tryalloc(ch);

	TEST_ASSERT_NULL(msg.payload);
	broker_adjust(broker, 1);
	msg = message_tryalloc(ch);
	TEST_ASSERT_NOT_NULL(msg.payload);
	publish(&msg);
	broker_destroy(broker);
}

TEST(broker, should_return_null)
{
	TEST_ASSERT_NULL(message_tryalloc(NULL).payload);
	TEST_ASSERT_NULL(message_getchannel((struct StMessage){NULL}));
	TEST_ASSERT_EQUAL_INT(0, broker_adjust(NULL, 10));
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

TEST_GROUP_RUNNER(broker)
{
	printf("BROKER TESTS\n");
	RUN_TEST_CASE(broker, should_destroy_null);
	RUN_TEST_CASE(broker, should_get_topic_from_channel);
	RUN_TEST_CASE(broker, should_return_null_payload_for_null_channel);
	RUN_TEST_CASE(broker, should_forward_message);
	RUN_TEST_CASE(broker, should_trace_null_subscriber);
	RUN_TEST_CASE(broker, should_unload_subscriber);
	RUN_TEST_CASE(broker, should_destroy_any_subscriber);
	RUN_TEST_CASE(broker, should_allow_zero_subscribers);
	if (!MEM_LIMITED)
		RUN_TEST_CASE(broker, should_allow_many_topics);
	RUN_TEST_CASE(broker, should_trace_null_broker);
	RUN_TEST_CASE(broker, should_alloc_message);
	RUN_TEST_CASE(broker, should_return_null);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
	if (MULTITHREADING)
		RUN_TEST_CASE(broker, should_support_multi_thread_pubsub);
}

static void run_basic_tests(void)
{
	RUN_TEST_GROUP(basis);
	RUN_TEST_GROUP(osal);
	RUN_TEST_GROUP(logger);
	RUN_TEST_GROUP(algo);
	RUN_TEST_GROUP(broker);
}

int main(int argc, const char** argv)
{
	SimpteMain(argc, argv, run_vertegs_tests);
	UnityMain(argc, argv, run_basic_tests);
	if (MULTITHREADING)
		SimpteMain(argc, argv, run_traffic_tests);
	return 0;
}
