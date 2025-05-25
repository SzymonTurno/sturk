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
TEST_SETUP(broker) { return; }
TEST_TEAR_DOWN(broker) { return; }

TEST(broker, should_support_single_thread_pubsub)
{
	const char* const expected[] = {
		EXPECTED_LINE("[info] broadcast -3")
		EXPECTED_LINE("[info] message: new = -3, old = 0")
		EXPECTED_LINE("[info] message: new = 0, old = 0")
		EXPECTED_LINE("[info] broadcast -13")
		EXPECTED_LINE("[info] message: new = -13, old = -3")
		EXPECTED_LINE("[info] message: new = 39, old = 0")
		EXPECTED_LINE("[info] broadcast 7")
		EXPECTED_LINE("[info] message: new = 7, old = -13")
		EXPECTED_LINE("[info] message: new = -91, old = 39")
		EXPECTED_LINE("[info] broadcast 1")
		EXPECTED_LINE("[info] message: new = 1, old = 7")
		EXPECTED_LINE("[info] message: new = 7, old = -91")
	};
	struct CyListener* listener = listener_create("broker_single_thread");

	log_attach(INFO, listener->stream);
	single_thread_pubsub();
	TEST_ASSERT_LISTENER_ORDERED(listener, expected);
	log_detach(INFO, listener->stream);
	listener_destroy(listener);
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
