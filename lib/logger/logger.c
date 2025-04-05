#include "ub/logger.h"
#include "outfile.h"
#include "ub/os/mutex.h"

static UBmutex* logger_mutex;

static void mutex_lock(void)
{
	const UBits policy = UB_MUTEX_POLICY_PRIO_INHERIT;
	const UBits type = UB_MUTEX_TYPE_RECURSIVE;

	if (!logger_mutex)
		logger_mutex = ub_mutex_create(policy | type);
	ub_mutex_lock(logger_mutex);
}

static void mutex_unlock(void)
{
	ub_mutex_unlock(logger_mutex);
}

static void mutex_destroy(void)
{
	UBmutex* tmp = logger_mutex;

	logger_mutex = NULL;
	if (tmp)
		ub_mutex_destroy(tmp);
}

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...)
{
	va_list vlist;
	va_list vcopy;
	char* msg = msg_create(lvl, tag, format, "");

	mutex_lock();
	va_start(vlist, format);
	switch (lvl) {
	default:
	case UB_INFO:
		if (outfile.fp)
			outfile_printf(msg, vlist);
		else
			ub_fvprintf(ub_stdout(), msg, vlist);
		break;
	case UB_DEBUG:
	case UB_WARNING:
	case UB_ERROR:
		if (outfile.fp) {
			va_copy(vcopy, vlist);
			outfile_printf(msg, vcopy);
			va_end(vcopy);
		}
		ub_fvprintf(ub_stdout(), msg, vlist);
		break;
	}
	va_end(vlist);
	mutex_unlock();
	ub_free(msg);
}

void ub_log_open(const char* filename)
{
	mutex_lock();
	if (outfile.fp) {
		ub_log_error("test already initialized");
	} else if (ub_fexists(filename)) {
		outfile.test_on = true;
		outfile.fp = ub_fopen(filename, "rb");
	} else {
		outfile.fp = ub_fopen(filename, "wb");
		outfile.cwd = ub_getcwd(NULL, 0);
		outfile.name = ub_malloc(strlen(filename) + 1);
		strcpy(outfile.name, filename);
	}
	mutex_unlock();
}

void ub_log_close(void)
{
	mutex_lock();
	if (outfile.fp) {
		ub_fclose(outfile.fp);
		outfile.fp = NULL;
		if (!outfile.test_on) {
			ub_log(UB_WARNING, LOGGER_TAG,
				"new file \"%s\" at \"%s\"", outfile.name,
				outfile.cwd);
			ub_free(outfile.name);
			outfile.name = NULL;
			ub_free(outfile.cwd);
			outfile.cwd = NULL;
		}
		outfile.test_on = false;
	} else {
		ub_log(UB_ERROR, LOGGER_TAG, "test not initialized");
	}
	mutex_unlock();
	mutex_destroy();
}
