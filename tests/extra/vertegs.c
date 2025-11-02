#include "simpte.h"
#include "sturk/cirq.h"
#include "sturk/graph.h"

TEST(vertegs, should_implement_cirq)
{
	VX_CIRQ(struct Cirq, void*);

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

TEST(vertegs, should_implement_rbtree)
{
	struct Vertegs* parent[VX_RB_N_EDGES] = {0};
	struct Vertegs* child[VX_RB_N_EDGES] = {0};

	TEST_ASSERT_EQUAL_PTR(
		vx_4nbor(child), vx_linkrb(vx_4nbor(child), vx_4nbor(parent)));
	TEST_ASSERT_EQUAL_PTR(
		vx_4nbor(parent), vx_postorderfirst(vx_4nbor(child)));
}

void run_vertegs_tests(void)
{
	RUN_TEST_CASE(vertegs, should_implement_cirq);
	RUN_TEST_CASE(vertegs, should_implement_rbtree);
}
