#include "algo.h"
#include "ub/os/mem.h"

struct StrList* strlist_ins(struct StrList* list, const char* str)
{
	struct StrList* self = ub_malloc(sizeof(*self));

	*list_data(self) = str;
	return list_ins(list, self);
}

const char* strlist_rem(struct StrList** listp)
{
	struct StrList* tmp = list_rem(listp);
	const char* ret = *list_data(tmp);

	ub_free(tmp);
	return ret;
}

struct StrQ* strq_ins(struct StrQ* q, const char* str)
{
	struct StrQ* self = ub_malloc(sizeof(*self));

	*ub_cirq_data(self) = str;
	return ub_cirq_ins(q, self);
}

const char* strq_rem(struct StrQ** qp)
{
	struct StrQ* tmp = ub_cirq_rem(qp);
	const char* ret = *ub_cirq_data(tmp);

	ub_free(tmp);
	return ret;
}
