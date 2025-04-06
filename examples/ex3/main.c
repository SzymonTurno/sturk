#include "ub/logger.h"
#include "ub/list.h"
#include "ub/os/mem.h"

UB_LIST(struct List, const char*);

static struct List* create_list(const char* str)
{
	struct List* self = ub_malloc(sizeof(*self));

	*ub_list_data(self) = str;
	return self;
}

int main(void)
{
	struct List* q = ub_list_ins(NULL, create_list("One"));
	struct List* tmp = NULL;

	q = ub_list_ins(q, create_list("Two"));
	q = ub_list_ins(q, create_list("Three"));
	ub_log_open(EXPECTED_OUT);
	while (q) {
		tmp = ub_list_rem(&q);
		ub_log(UB_INFO, NULL, *ub_list_data(tmp));
		ub_free(tmp);
	}
 	ub_log_close();
	return 0;
}
