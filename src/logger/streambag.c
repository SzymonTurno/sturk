#include "cantil/logger/streambag.h"
#include "cantil/list.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include "cantil/os/mutex.h"

LIST(struct StreamList, CnFstream*);

struct CnStreambag {
	struct StreamList* head;
	CnMutex* mutex;
};

static void
list_print(struct StreamList* head, const char* format, va_list vlist)
{
	va_list vcopy;

	list_iter (i, &head) {
		va_copy(vcopy, vlist);
		cn_fvprintf(*list_data(*i), format, vlist);
		va_end(vcopy);
	}
}

CnStreambag* cn_streambag_create(void)
{
	CnStreambag* self = NEW(CnStreambag);

	self->head = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void cn_streambag_destroy(CnStreambag* bag)
{
	if (!bag)
		return;
	mutex_destroy(bag->mutex);
	bag->mutex = NULL;
	while (bag->head)
		cn_free(list_rem(&bag->head));
	cn_free(bag);
}

void cn_streambag_ins(CnStreambag* bag, CnFstream* stream)
{
	struct StreamList* entry = NEW(struct StreamList);

	ENSURE(bag, ERROR, null_param);
	*list_data(entry) = stream;
	mutex_lock(bag->mutex);
	bag->head = list_ins(bag->head, entry);
	mutex_unlock(bag->mutex);
}

void cn_streambag_rem(CnStreambag* bag, CnFstream* stream)
{
	ENSURE(bag, ERROR, null_param);
	list_iter (i, &bag->head)
		if (*list_data(*i) == stream) {
			mutex_lock(bag->mutex);
			cn_free(list_rem(i));
			mutex_unlock(bag->mutex);
			break;
		}
}

void cn_streambag_vprint(CnStreambag* bag, const char* format, va_list vlist)
{
	ENSURE(bag, ERROR, null_param);
	mutex_lock(bag->mutex);
	list_print(bag->head, format, vlist);
	mutex_unlock(bag->mutex);
}
