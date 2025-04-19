#include "unity.h"
#include "UB/debug/log.h"

extern void test_List_should_ImplementLifo(void);
extern void test_Queue_should_ImplementFifo(void);
extern void test_Broker_should_SupportMultiThreadDesign(void);

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
	UNITY_BEGIN();
	log_attach(WARNING, ub_stderr());
	log_attach(ERROR, ub_stderr());
	RUN_TEST(test_List_should_ImplementLifo);
	RUN_TEST(test_Queue_should_ImplementFifo);
	RUN_TEST(test_Broker_should_SupportMultiThreadDesign);
	log_deinit();
	return UNITY_END();
}
