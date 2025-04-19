#include "UB/debug/log.h"
#include "UB/debug/logger.h"
#include "UB/debug/except.h"
#include "ub/os/mem.h"
#include "types.h"

static struct UBlogger* logger[N_LOG_LVLS];

static const char* get_lvlstr(enum UBlogLvl lvl)
{
	switch (lvl) {
	case DEBUG:
		return "debug";
	case INFO:
		return "info";
	case WARNING:
		return "warning";
	case ERROR:
		return "error";
	default:
		RAISE(ECODES.not_supported);
		break;
	}
	return "UNKNOWN LOG LEVEL";
}

void ub_log_attach(enum UBlogLvl lvl, UBfstream* stream)
{
	if (!logger[lvl])
		logger[lvl] = logger_create();
	logger_attach(logger[lvl], stream);
}

void ub_log_detach(enum UBlogLvl lvl, UBfstream* stream)
{
	logger_detach(logger[lvl], stream);
}

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	char* buff = NULL;

	if (!logger[lvl])
		return;
	buff = ub_malloc(BUFF_MAX_SIZE);
	if (tag)
		ub_snprintf(buff, BUFF_MAX_SIZE, "[%s][%s] %s\n",
			get_lvlstr(lvl), tag, format);
	else
		ub_snprintf(buff, BUFF_MAX_SIZE, "[%s] %s\n",
			get_lvlstr(lvl), format);
	va_start(vlist, format);
	logger_vprint(logger[lvl], buff, vlist);
	va_end(vlist);
	ub_free(buff);
}

void ub_log_deinit(void)
{
	for (int i = 0; i < N_LOG_LVLS; i++) {
		logger_destroy(logger[i]);
		logger[i] = NULL;
	}
}
