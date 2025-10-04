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

#include "basis/io.h"
#include "vertegs/vertex.h"

struct Buffer {
	size_t front;
	size_t rear;
};

static void stream_putc(void* p, char c)
{
	struct Buffer* buff = p;
	char* stream = (char*)&buff[1];

	stream[buff->front++] = c;
}

static char stream_getc(void* p)
{
	struct Buffer* buff = p;
	char* stream = (char*)&buff[1];

	return (buff->front == buff->rear) ? IO_EOF : stream[buff->rear++];
}

static const struct StIoVtable VTABLE[] = {
	{.putc_cb = stream_putc, .getc_cb = stream_getc}};

StIo* st_io_init(StMemBuff* buff)
{
	StIo* io = (StIo*)buff;
	struct Buffer* pb = NULL;

	VX_ENSURE_MEM(buff);
	io->s.vp = VTABLE;
	pb = (struct Buffer*)&io[1];
	pb->front = 0;
	pb->rear = 0;
	return io;
}

void st_io_setp(StIo* io, void* p)
{
	VX_ASSERT(io);
	io->s.u.flags |= USR_MODE_MASK;
	*(void**)&io[1] = p;
}

void st_io_setvp(StIo* io, const struct StIoVtable* vp)
{
	VX_ASSERT(io);
	io->s.vp = vp;
}

char* st_io_fgets(char* str, int size, StIo* io)
{
	char c = '\0';
	char* ret = str;

	VX_ENSURE_MEM(str);
	VX_ASSERT(size > 0);
	while (--size && c != '\n' && (c = iogetc(io)) != IO_EOF)
		*str++ = c;
	*str = '\0';
	return ret;
}
