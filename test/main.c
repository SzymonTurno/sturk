#include "unittest.h"
#include "algo.h"
#include "pubsub.h"
#include <string.h>

TEST_GROUP(list);
TEST_SETUP(list) { return; }
TEST_TEAR_DOWN(list) { return; }

TEST(list, should_implement_lifo)
{
	struct StrList* list = NULL;

	list = strlist_ins(NULL, "One");
	list = strlist_ins(list, "Two");
	list = strlist_ins(list, "Three");
	TEST_ASSERT_EQUAL(0, strcmp(strlist_rem(&list), "Three"));
	TEST_ASSERT_EQUAL(0, strcmp(strlist_rem(&list), "Two"));
	TEST_ASSERT_EQUAL(0, strcmp(strlist_rem(&list), "One"));
	TEST_ASSERT_EQUAL(list, NULL);
}

TEST_GROUP(cirq);
TEST_SETUP(cirq) { return; }
TEST_TEAR_DOWN(cirq) { return; }

TEST(cirq, should_implement_fifo)
{
	struct StrQ* q = NULL;

	q = strq_ins(NULL, "One");
	q = strq_ins(q, "Two");
	q = strq_ins(q, "Three");
	TEST_ASSERT_EQUAL(0, strcmp(strq_rem(&q), "One"));
	TEST_ASSERT_EQUAL(0, strcmp(strq_rem(&q), "Two"));
	TEST_ASSERT_EQUAL(0, strcmp(strq_rem(&q), "Three"));
	TEST_ASSERT_EQUAL(q, NULL);
}

TEST_GROUP(broker);

TEST_SETUP(broker)
{
	snapshot_init(ACTUAL_OUT);

	log_attach(INFO, snapshot_get_stream());
}

TEST_TEAR_DOWN(broker)
{
	log_detach(INFO, snapshot_get_stream());
	snapshot_deinit();
}

TEST(broker, should_support_single_thread_pubsub)
{
	single_thread_pubsub();
	TEST_ASSERT_SNAPSHOT_ORDERED(EXPECTED_OUT);
}

static void run_all_tests(void)
{
	RUN_TEST_CASE(list, should_implement_lifo);
	RUN_TEST_CASE(cirq, should_implement_fifo);
	RUN_TEST_CASE(broker, should_support_single_thread_pubsub);
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
