/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cantil/logger/trace.h"
#include "cantil/logger/except.h"
#include "cantil/logger/streambag.h"
#include "cantil/os/mem.h"

#define BUFF_MAX_SIZE 128

static struct CnStreamBag* streambags[N_TRACE_LVLS];

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
