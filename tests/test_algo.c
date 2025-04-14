#include "unity.h"
#include "UB/list.h"
#include "ub/cirq.h"
#include "ub/os/mem.h"
#include <string.h>

LIST(struct List, const char*);

UB_CIRQ(struct Queue, const char*);

static struct List* insert_list(struct List* list, const char* str)
{
	struct List* self = ub_malloc(sizeof(*self));

	*list_data(self) = str;
	return list_ins(list, self);
}

static const char* remove_list(struct List** listp)
{
	struct List* tmp = list_rem(listp);
	const char* ret = *list_data(tmp);

	ub_free(tmp);
	return ret;
}

static struct Queue* insert_queue(struct Queue* q, const char* str)
{
	struct Queue* self = ub_malloc(sizeof(*self));

	*ub_cirq_data(self) = str;
	return ub_cirq_ins(q, self);
}

static const char* remove_queue(struct Queue** qp)
{
	struct Queue* tmp = ub_cirq_rem(qp);
	const char* ret = *ub_cirq_data(tmp);

	ub_free(tmp);
	return ret;
}

static void test_List_should_ImplementLifo(void)
{
	struct List* list = NULL;

	list = insert_list(NULL, "One");
	list = insert_list(list, "Two");
	list = insert_list(list, "Three");
	TEST_ASSERT_EQUAL(0, strcmp(remove_list(&list), "Three"));
	TEST_ASSERT_EQUAL(0, strcmp(remove_list(&list), "Two"));
	TEST_ASSERT_EQUAL(0, strcmp(remove_list(&list), "One"));
	TEST_ASSERT_EQUAL(list, NULL);
}

static void test_Queue_should_ImplementFifo(void)
{
	struct Queue* q = NULL;

	q = insert_queue(NULL, "One");
	q = insert_queue(q, "Two");
	q = insert_queue(q, "Three");
	TEST_ASSERT_EQUAL(0, strcmp(remove_queue(&q), "One"));
	TEST_ASSERT_EQUAL(0, strcmp(remove_queue(&q), "Two"));
	TEST_ASSERT_EQUAL(0, strcmp(remove_queue(&q), "Three"));
	TEST_ASSERT_EQUAL(q, NULL);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_List_should_ImplementLifo);
	RUN_TEST(test_Queue_should_ImplementFifo);
	return UNITY_END();
}
