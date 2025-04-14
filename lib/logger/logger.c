#include "UB/logger.h"
#include "outfile.h"
#include "UB/os/mutex.h"

static UBmutex* logger_mutex;

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	va_list vcopy;
	char* msg = msg_create(lvl, tag, format, "");

	if (logger_mutex)
		mutex_lock(logger_mutex);
	va_start(vlist, format);
	switch (lvl) {
	default:
	case INFO:
		if (outfile.fp)
			outfile_printf(msg, vlist);
		else
			ub_fvprintf(ub_stdout(), msg, vlist);
		break;
	case DEBUG:
		if (outfile.fp) {
			va_copy(vcopy, vlist);
			outfile_printf(msg, vcopy);
			va_end(vcopy);
		}
		ub_fvprintf(ub_stdout(), msg, vlist);
		break;
	case WARNING:
	case ERROR:
		if (outfile.fp) {
			va_copy(vcopy, vlist);
			outfile_printf(msg, vcopy);
			va_end(vcopy);
		}
		ub_fvprintf(ub_stderr(), msg, vlist);
		break;
	}
	va_end(vlist);
	if (logger_mutex)
		mutex_unlock(logger_mutex);
	ub_free(msg);
}

void ub_log_open(const char* filename)
{
	logger_mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	if (outfile.fp) {
		LOG(ERROR, LOGGER_TAG, "test already initialized");
	} else if (ub_fexists(filename)) {
		outfile.u.test_on = true;
		outfile.fp = ub_fopen(filename, "rb");
	} else {
		outfile.fp = ub_fopen(filename, "wb");
		outfile.cwd = ub_getcwd(NULL, 0);
		outfile.name = ub_malloc(strlen(filename) + 1);
		strcpy(outfile.name, filename);
	}
}

void ub_log_close(void)
{
	if (outfile.fp) {
		ub_fclose(outfile.fp);
		outfile.fp = NULL;
		if (!outfile.u.test_on) {
			LOG(WARNING, LOGGER_TAG, "new file \"%s\" at \"%s\"",
				outfile.name, outfile.cwd);
			ub_free(outfile.name);
			outfile.name = NULL;
			ub_free(outfile.cwd);
			outfile.cwd = NULL;
		}
		outfile.u.test_on = false;
	}
	mutex_destroy(logger_mutex);
	logger_mutex = NULL;
}
