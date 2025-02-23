#ifndef OUTFILE_H
#define OUTFILE_H

#include "message.h"
#include <string.h>
#include <ctype.h>

#define LOGGER_TAG "ub-logger"

static struct {
	UBfstream* fp;
	char* cwd;
	char* name;
	int test_on;
	int _padding;
} outfile;

static inline void rtrim(char* str)
{
	int p = 0;

	if (str && *str) {
		while (str[++p])
			;
		while (isspace(str[--p]))
			str[p] = '\0';
	}
}

static inline void skip_spaces(UBfstream* stream)
{
	int c = 0;

	while (isspace(c = ub_fs_fgetc(stream)))
		;
	ub_fs_fseekcur(stream, -1);
}

static inline void outfile_err(const char* s1, const char* s2)
{
	va_list vlist;
	char* msg = msg_create(UB_ERROR, LOGGER_TAG, s1, s2);

	ub_fs_vprintf(ub_fs_stderr(), msg, vlist);
	ub_free(msg);
}

static inline void outfile_verify(const char* format, va_list vlist)
{
	char* logged = ub_malloc(LOG_MAX_LENGTH);
	char* expect = ub_malloc(LOG_MAX_LENGTH);

	ub_vsnprintf(logged, LOG_MAX_LENGTH, format, vlist);
	skip_spaces(outfile.fp);
	ub_fs_fgets(expect, LOG_MAX_LENGTH, outfile.fp);
	rtrim(logged);
	rtrim(expect);
	if (strcmp(logged, expect)) {
		outfile_err("logged: ", logged);
		outfile_err("expected: ", expect);
	}
	ub_free(expect);
	ub_free(logged);
}

static inline void outfile_printf(const char* format, va_list vlist)
{
	if (outfile.test_on)
		outfile_verify(format, vlist);
	else
		ub_fs_vprintf(outfile.fp, format, vlist);
}

#endif /* OUTFILE_H */
