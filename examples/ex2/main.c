#include "ub/logger.h"
#include "ub/cirq.h"
#include "ub/os/mem.h"

UB_CIRQ(struct Queue, const char*);

static struct Queue* create_queue(const char* str)
{
	struct Queue* self = ub_malloc(sizeof(*self));

	*ub_cirq_data(self) = str;
	return self;
}

int main(void)
{
	struct Queue* q = ub_cirq_ins(NULL, create_queue("One"));
	struct Queue* tmp = NULL;

	q = ub_cirq_ins(q, create_queue("Two"));
	q = ub_cirq_ins(q, create_queue("Three"));
	ub_log_open(EXPECTED_OUT);
	while (q) {
		tmp = ub_cirq_rem(&q);
		ub_log(UB_INFO, NULL, *ub_cirq_data(tmp));
		ub_free(tmp);
	}
 	ub_log_close();
	return 0;
}
