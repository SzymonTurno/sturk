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

#include "sturk/logger/trace.h"
#include "sturk/io/bag.h"
#include "sturk/os/mem.h"
#include "sturk/os/sys.h"

#define BUFF_MAX_SIZE 128

static struct StIoBag* iobags[N_TRACE_LVLS];

static const char* getlvlstr(enum StTraceLvl lvl)
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
	except(st_except_sanity_fail.reason, __FILE__, __LINE__);
	return "unknown trace level";
	/* LCOV_EXCL_STOP */
}

void st_trace(enum StTraceLvl lvl, const char* tag, const char* fmt, ...)
{
	va_list va;
	char* buff = NULL;

	/* LCOV_EXCL_START */
	if (lvl <= UNKNOWN || lvl >= N_TRACE_LVLS)
		except(st_except_not_supported.reason, __FILE__, __LINE__);
	/* LCOV_EXCL_STOP */

	if (!iobag_count(iobags[lvl]))
		return;
	buff = NEW(char, BUFF_MAX_SIZE);
	if (tag)
		st_strprint(buff, "[%s][%s] %s\n", getlvlstr(lvl), tag, fmt);
	else
		st_strprint(buff, "[%s] %s\n", getlvlstr(lvl), fmt);
	va_start(va, fmt);
	iobag_vprint(iobags[lvl], buff, va);
	va_end(va);
	st_free(buff);
}

void st_logger_attach(enum StTraceLvl lvl, StIo* io)
{
	if (!iobags[lvl])
		iobags[lvl] = iobag_create();
	iobag_ins(iobags[lvl], io);
}

void st_logger_detach(enum StTraceLvl lvl, StIo* io)
{
	iobag_rem(iobags[lvl], io);
}

void st_logger_cleanup(void)
{
	for (int i = 0; i < N_TRACE_LVLS; i++) {
		iobag_destroy(iobags[i]);
		iobags[i] = NULL;
	}
}
