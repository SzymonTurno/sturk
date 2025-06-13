#include "cantil/logger/sink.h"
#include "cantil/list.h"
#include "cantil/logger/except.h"
#include "cantil/os/mutex.h"
#include "cn/os/mem.h"

LIST(struct StreamList, CnFstream*);

struct CnLogsink {
	struct StreamList* head;
	CnMutex* mutex;
};

static void
list_print(struct StreamList* head, const char* format, va_list vlist)
{
	va_list vcopy;

	for (LIST_ITER(struct StreamList, i, &head)) {
		va_copy(vcopy, vlist);
		cn_fvprintf(*list_data(*i), format, vlist);
		va_end(vcopy);
	}
}

CnLogsink* cn_logsink_create(void)
{
	CnLogsink* self = cn_malloc(sizeof(*self));

	self->head = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void cn_logsink_ins(CnLogsink* list, CnFstream* stream)
{
	struct StreamList* entry = cn_malloc(sizeof(*entry));

	ENSURE(list, EXCEPT.NULL_PARAM);
	*list_data(entry) = stream;
	mutex_lock(list->mutex);
	list->head = list_ins(list->head, entry);
	mutex_unlock(list->mutex);
}

void cn_logsink_rem(CnLogsink* list, CnFstream* stream)
{
	ENSURE(list, EXCEPT.NULL_PARAM);
	for (LIST_ITER(struct StreamList, i, &list->head))
		if (*list_data(*i) == stream) {
			mutex_lock(list->mutex);
			cn_free(list_rem(i));
			mutex_unlock(list->mutex);
			break;
		}
}

void cn_logsink_vprint(CnLogsink* list, const char* format, va_list vlist)
{
	ENSURE(list, EXCEPT.NULL_PARAM);
	mutex_lock(list->mutex);
	list_print(list->head, format, vlist);
	mutex_unlock(list->mutex);
}

void cn_logsink_destroy(CnLogsink* list)
{
	if (!list)
		return;
	mutex_destroy(list->mutex);
	list->mutex = NULL;
	while (list->head)
		cn_free(list_rem(&list->head));
	cn_free(list);
}
