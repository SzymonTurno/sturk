#include "UB/debug/logger.h"
#include "UB/debug/except.h"
#include "UB/list.h"
#include "UB/os/mutex.h"
#include "ub/os/mem.h"

LIST(struct StreamList, UBfstream*);

struct UBlogger {
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

UBlogger* ub_logger_create(void)
{
	UBlogger* self = ub_malloc(sizeof(*self));

	self->list = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void ub_logger_attach(UBlogger* logger, UBfstream* stream)
{
	struct StreamList* entry = ub_malloc(sizeof(*entry));

	ENSURE(logger, ECODES.null_param);
	*list_data(entry) = stream;
	mutex_lock(logger->mutex);
	logger->list = list_ins(logger->list, entry);
	mutex_unlock(logger->mutex);
}

void ub_logger_detach(UBlogger* logger, UBfstream* stream)
{
	ENSURE(logger, ECODES.null_param);
	for (LIST_ITER(struct StreamList, i, &logger->list))
		if (*list_data(*i) == stream) {
			mutex_lock(logger->mutex);
			ub_free(list_rem(i));
			mutex_unlock(logger->mutex);
			break;
		}
}

void ub_logger_vprint(UBlogger* logger, const char* format, va_list vlist)
{
	ENSURE(logger, ECODES.null_param);
	mutex_lock(logger->mutex);
	slprint(logger->list, format, vlist);
	mutex_unlock(logger->mutex);
}

void ub_logger_print(UBlogger* logger, const char* format, ...)
{
	va_list vlist;

	va_start(vlist, format);
	logger_vprint(logger, format, vlist);
	va_end(vlist);
}

void ub_logger_destroy(UBlogger* logger)
{
	if (!logger)
		return;
	mutex_destroy(logger->mutex);
	logger->mutex = NULL;
	while (logger->list)
		ub_free(list_rem(&logger->list));
	ub_free(logger);
}
