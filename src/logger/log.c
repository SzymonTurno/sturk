#include "UB/logger/log.h"
#include "UB/logger/streambag.h"
#include "UB/logger/except.h"
#include "ub/os/mem.h"

#define BUFF_MAX_SIZE 128

static struct CyStreambag* streambag[N_LOG_LVLS];

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
	if (!streambag[lvl])
		streambag[lvl] = streambag_create();
	streambag_push(streambag[lvl], stream);
}

void ub_log_detach(enum UBlogLvl lvl, UBfstream* stream)
{
	streambag_pop(streambag[lvl], stream);
}

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	char* buff = NULL;

	if (!streambag[lvl])
		return;
	buff = ub_malloc(BUFF_MAX_SIZE);
	if (tag)
		ub_snprintf(buff, BUFF_MAX_SIZE, "[%s][%s] %s\n",
			get_lvlstr(lvl), tag, format);
	else
		ub_snprintf(buff, BUFF_MAX_SIZE, "[%s] %s\n",
			get_lvlstr(lvl), format);
	va_start(vlist, format);
	streambag_vprint(streambag[lvl], buff, vlist);
	va_end(vlist);
	ub_free(buff);
}

void ub_log_cleanup(void)
{
	for (int i = 0; i < N_LOG_LVLS; i++) {
		streambag_destroy(streambag[i]);
		streambag[i] = NULL;
	}
}
