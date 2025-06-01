#include "UB/logger/sink.h"
#include "UB/logger/except.h"
#include "UB/list.h"
#include "UB/os/mutex.h"
#include "ub/os/mem.h"

LIST(struct StreamList, UBfstream*);

struct CnLogsink {
	struct StreamList* head;
	UBmutex* mutex;
};

static void
list_print(struct StreamList* head, const char* format, va_list vlist)
{
	va_list vcopy;

	for (LIST_ITER(struct StreamList, i, &head)) {
		va_copy(vcopy, vlist);
		ub_fvprintf(*list_data(*i), format, vlist);
		va_end(vcopy);
	}
}

CnLogsink* cn_logsink_create(void)
{
	CnLogsink* self = ub_malloc(sizeof(*self));

	self->head = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void cn_logsink_push(CnLogsink* list, UBfstream* stream)
{
	struct StreamList* entry = ub_malloc(sizeof(*entry));

	ENSURE(list, ECODES.null_param);
	*list_data(entry) = stream;
	mutex_lock(list->mutex);
	list->head = list_ins(list->head, entry);
	mutex_unlock(list->mutex);
}

void cn_logsink_pop(CnLogsink* list, UBfstream* stream)
{
	ENSURE(list, ECODES.null_param);
	for (LIST_ITER(struct StreamList, i, &list->head))
		if (*list_data(*i) == stream) {
			mutex_lock(list->mutex);
			ub_free(list_rem(i));
			mutex_unlock(list->mutex);
			break;
		}
}

void cn_logsink_vprint(CnLogsink* list, const char* format, va_list vlist)
{
	ENSURE(list, ECODES.null_param);
	mutex_lock(list->mutex);
	list_print(list->head, format, vlist);
	mutex_unlock(list->mutex);
}

void cn_logsink_print(CnLogsink* list, const char* format, ...)
{
	va_list vlist;

	va_start(vlist, format);
	logsink_vprint(list, format, vlist);
	va_end(vlist);
}

void cn_logsink_destroy(CnLogsink* list)
{
	if (!list)
		return;
	mutex_destroy(list->mutex);
	list->mutex = NULL;
	while (list->head)
		ub_free(list_rem(&list->head));
	ub_free(list);
}
