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

/**
 * @file st/io/buffer.h
 *
 * @brief Input/output buffer.
 */

#ifndef ST_IO_BUFFER_H
#define ST_IO_BUFFER_H

#include "st/arith.h"
#include <stdarg.h>

#define ST_IO_EOF -1

#define st_iobuffer_calclen(bytes)                                             \
	(3 + (sizeof(char*) + (bytes) - 1) / sizeof(StAlign))

typedef StAlign StIoBuffer;

typedef union StIo StIo;

struct StIoVt {
	void (*putc_cb)(void*, char);
	char (*getc_cb)(void*);
};

StIo* st_io_init(StIoBuffer* buff);

void st_io_setp(StIo* io, void* p);

void st_io_setvp(StIo* io, const struct StIoVt* vp);

void st_io_putc(StIo* io, char c);

char st_io_getc(StIo* io);

void st_io_vprint(StIo* io, const char* fmt, va_list va);

void st_io_print(StIo* io, const char* fmt, ...);

char* st_io_fgets(char* str, int size, StIo* io);

#endif /* ST_IO_BUFFER_H */
