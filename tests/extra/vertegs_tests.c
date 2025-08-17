#include "cantil/cirq.h"
#include "cantil/graph.h"
#include <assert.h>
#include <stdio.h>

#define TEST(group, case) static void test_##group##_##case (void)

#define SIMPLE_TEST_GROUP(group, label)                                        \
	static int test_##group##_counter;                                     \
	static const char* test_##group##_label = label

/* clang-format off */
#define RUN_TEST_CASE(group, case)                                             \
	do {                                                                   \
		printf(".\nRunning test case %d for: %s.\n",                   \
		       ++test_##group##_counter, test_##group##_label);        \
		test_##group##_##case();                                       \
		printf("Done\n");                                              \
	} while (0)
/* clang-format on */

#define TEST_ASSERT_EQUAL_PTR(expected, actual)                                \
	assert((void*)(expected) == (void*)(actual))

SIMPLE_TEST_GROUP(vertegs, "vertegs");

TEST(vertegs, should_implement_cirq)
{
	GRAPH(struct Cirq, 2, void*);

	const size_t next = 0;
	const size_t prev = 1;
	struct Cirq n[5] = {0};
	struct Cirq* head = NULL;

	/* -n0- */
	TEST_ASSERT_EQUAL_PTR(&n[0], cirq_ins((struct Cirq*)NULL, &n[0], 255));
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[next], &n[0]);

	/* -n1--n0- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[0], &n[1], 0));
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[next], &n[1]);

	/* -n1--n0--n2- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[2], -1));
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[next], &n[1]);

	/* -n1--n3--n0--n- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[3], 1));
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[next], &n[3]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[3]))->nbor[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[3]))->nbor[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[prev], &n[3]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[next], &n[1]);

	/* -n1--n3--n0--n--n2- */
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_ins(&n[1], &n[4], -2));
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[prev], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[1]))->nbor[next], &n[3]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[3]))->nbor[prev], &n[1]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[3]))->nbor[next], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[prev], &n[3]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[0]))->nbor[next], &n[4]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[4]))->nbor[prev], &n[0]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[4]))->nbor[next], &n[2]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[prev], &n[4]);
	TEST_ASSERT_EQUAL_PTR((graph_2vx(&n[2]))->nbor[next], &n[1]);

	head = &n[1];
	TEST_ASSERT_EQUAL_PTR(&n[4], cirq_rem(&head, -2));
	TEST_ASSERT_EQUAL_PTR(&n[3], cirq_rem(&head, 1));
	TEST_ASSERT_EQUAL_PTR(&n[2], cirq_rem(&head, -1));
	TEST_ASSERT_EQUAL_PTR(&n[1], cirq_rem(&head, 0));
	TEST_ASSERT_EQUAL_PTR(&n[0], cirq_rem(&head, 255));
	TEST_ASSERT_EQUAL_PTR(NULL, head);
}

void run_vertegs_tests(void)
{
	RUN_TEST_CASE(vertegs, should_implement_cirq);
}
