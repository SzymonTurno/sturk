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

#include "sturk/io/buffer.h"
#include "sturk/debug.h"

static void buffer_putc(struct StIo* vp, char c)
{
	struct StIoBuffer* buff = (struct StIoBuffer*)vp;

	ASSERT(buff);
	ASSERT(buff->cp);
	*buff->cp++ = c;
}

static char buffer_getc(struct StIo* vp)
{
	struct StIoBuffer* buff = (struct StIoBuffer*)vp;
	char c = 0;

	ASSERT(buff);
	ASSERT(buff->cp);
	c = *buff->cp;
	if (c != IO_EOF)
		++buff->cp;
	return c;
}

struct StIo* st_iobuffer_init(struct StIoBuffer* buff)
{
	struct StIo* ret = (struct StIo*)buff;

	VX_ENSURE_MEM(ret);
	/* Ensure `struct StIo` is the first member of `struct StIoBuffer`. */
	ASSERT(ret == &buff->_vt);
	ret->putc_cb = buffer_putc;
	ret->getc_cb = buffer_getc;
	buff->cp = (char*)&buff[1];
	return ret;
}
