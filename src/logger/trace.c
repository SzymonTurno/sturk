#include "cantil/logger/trace.h"
#include "cantil/logger/except.h"
#include "cantil/logger/streambag.h"
#include "cantil/os/mem.h"

#define BUFF_MAX_SIZE 128

static struct CnStreambag* streambags[N_TRACE_LVLS];

static const char* get_lvlstr(enum CnTraceLvl lvl)
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
	/* LCOV_EXCL_START */
	default:
		break;
	}
	RAISE(ERROR, sanity_fail);
	return "unknown trace level";
	/* LCOV_EXCL_STOP */
}

void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	char* buff = NULL;

	ENSURE(lvl > UNKNOWN && lvl < N_TRACE_LVLS, ERROR, not_supported);
	if (!streambags[lvl])
		return;
	buff = NEW(char, BUFF_MAX_SIZE);
	if (tag)
		cn_snprintf(
			buff, BUFF_MAX_SIZE, "[%s][%s] %s\n", get_lvlstr(lvl),
			tag, format);
	else
		cn_snprintf(
			buff, BUFF_MAX_SIZE, "[%s] %s\n", get_lvlstr(lvl),
			format);
	va_start(vlist, format);
	streambag_vprint(streambags[lvl], buff, vlist);
	va_end(vlist);
	cn_free(buff);
}

void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream)
{
	if (!streambags[lvl])
		streambags[lvl] = streambag_create();
	streambag_ins(streambags[lvl], stream);
}

void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream)
{
	streambag_rem(streambags[lvl], stream);
}

void cn_logger_cleanup(void)
{
	for (int i = 0; i < N_TRACE_LVLS; i++) {
		streambag_destroy(streambags[i]);
		streambags[i] = NULL;
	}
}
