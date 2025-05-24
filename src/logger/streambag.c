#include "UB/logger/streambag.h"
#include "UB/logger/except.h"
#include "UB/list.h"
#include "UB/os/mutex.h"
#include "ub/os/mem.h"

LIST(struct StreamList, UBfstream*);

struct CyStreambag {
	struct StreamList* list;
	UBmutex* mutex;
};

static void slprint(struct StreamList* list, const char* format, va_list vlist)
{
	va_list vcopy;

	for (LIST_ITER(struct StreamList, i, &list)) {
		va_copy(vcopy, vlist);
		ub_fvprintf(*list_data(*i), format, vlist);
		va_end(vcopy);
	}
}

CyStreambag* cy_streambag_create(void)
{
	CyStreambag* self = ub_malloc(sizeof(*self));

	self->list = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void cy_streambag_push(CyStreambag* bag, UBfstream* stream)
{
	struct StreamList* entry = ub_malloc(sizeof(*entry));

	ENSURE(bag, ECODES.null_param);
	*list_data(entry) = stream;
	mutex_lock(bag->mutex);
	bag->list = list_ins(bag->list, entry);
	mutex_unlock(bag->mutex);
}

void cy_streambag_pop(CyStreambag* bag, UBfstream* stream)
{
	ENSURE(bag, ECODES.null_param);
	for (LIST_ITER(struct StreamList, i, &bag->list))
		if (*list_data(*i) == stream) {
			mutex_lock(bag->mutex);
			ub_free(list_rem(i));
			mutex_unlock(bag->mutex);
			break;
		}
}

void cy_streambag_vprint(CyStreambag* bag, const char* format, va_list vlist)
{
	ENSURE(bag, ECODES.null_param);
	mutex_lock(bag->mutex);
	slprint(bag->list, format, vlist);
	mutex_unlock(bag->mutex);
}

void cy_streambag_print(CyStreambag* bag, const char* format, ...)
{
	va_list vlist;

	va_start(vlist, format);
	streambag_vprint(bag, format, vlist);
	va_end(vlist);
}

void cy_streambag_destroy(CyStreambag* bag)
{
	if (!bag)
		return;
	mutex_destroy(bag->mutex);
	bag->mutex = NULL;
	while (bag->list)
		ub_free(list_rem(&bag->list));
	ub_free(bag);
}
