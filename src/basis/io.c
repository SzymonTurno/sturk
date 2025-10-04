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

static void st_putc(void* p, char c)
{
	char** cp = p;

	*(*cp)++ = c;
}

const struct StIoVtable IO_BUFF_VTABLE[] = {{.putc_cb = st_putc}};

void st_iovtable_init(struct StIoVtable* vp)
{
	VX_ASSERT(vp);
	*vp = *IO_BUFF_VTABLE;
}

StIo* st_io_init(void* buff)
{
	((StIo*)buff)->s.vp = IO_BUFF_VTABLE;
	return buff;
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
