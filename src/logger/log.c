#include "cantil/logger/log.h"
#include "cantil/logger/sink.h"
#include "cantil/logger/except.h"
#include "cn/os/mem.h"

#define BUFF_MAX_SIZE 128

static struct CnLogsink* logsink[N_LOG_LVLS];

static const char* get_lvlstr(enum CnLogLvl lvl)
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
		break;
	}
	RAISE(ECODES.not_supported);
	return "UNKNOWN LOG LEVEL";
}

void cn_log_attach(enum CnLogLvl lvl, CnFstream* stream)
{
	if (!logsink[lvl])
		logsink[lvl] = logsink_create();
	logsink_ins(logsink[lvl], stream);
}

void cn_log_detach(enum CnLogLvl lvl, CnFstream* stream)
{
	logsink_rem(logsink[lvl], stream);
}

void cn_log(enum CnLogLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	char* buff = NULL;

	if (!logsink[lvl])
		return;
	buff = cn_malloc(BUFF_MAX_SIZE);
	if (tag)
		cn_snprintf(buff, BUFF_MAX_SIZE, "[%s][%s] %s\n",
			get_lvlstr(lvl), tag, format);
	else
		cn_snprintf(buff, BUFF_MAX_SIZE, "[%s] %s\n",
			get_lvlstr(lvl), format);
	va_start(vlist, format);
	logsink_vprint(logsink[lvl], buff, vlist);
	va_end(vlist);
	cn_free(buff);
}

void cn_log_cleanup(void)
{
	for (int i = 0; i < N_LOG_LVLS; i++) {
		logsink_destroy(logsink[i]);
		logsink[i] = NULL;
	}
}
