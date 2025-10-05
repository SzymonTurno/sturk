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
#include <limits.h>

static char ul2c(unsigned long num)
{
	VX_ASSERT(num <= CHAR_MAX);
	return (char)num;
}

static unsigned c2u(char c)
{
	VX_ASSERT(c >= 0);
	return (unsigned)c;
}

static void ul2a(unsigned long num, unsigned base, int uc, char* bf)
{
	const char a = uc ? 'A' : 'a';
	int n = 0;
	unsigned d = 1;
	unsigned long dgt = 0;

	while (num / d >= base)
		d *= base;

	while (d != 0) {
		dgt = num / d;
		num %= d;
		d /= base;
		if (n || dgt || d == 0) {
			*bf++ = ul2c(dgt + (dgt < 10 ? '0' : c2u(a - 10)));
			++n;
		}
	}
	*bf = 0;
}

static void li2a(long num, char* bf)
{
	if (num < 0) {
		num = -num;
		*bf++ = '-';
	}
	ul2a((unsigned long)num, 10, 0, bf);
}

static void ui2a(unsigned int num, unsigned base, int uc, char* bf)
{
	const char a = uc ? 'A' : 'a';
	int n = 0;
	unsigned d = 1;
	unsigned dgt = 0;

	while (num / d >= base)
		d *= base;

	while (d != 0) {
		dgt = num / d;
		num %= d;
		d /= base;
		if (n || dgt > 0 || d == 0) {
			*bf++ = ul2c(dgt + (dgt < 10 ? '0' : c2u(a - 10)));
			++n;
		}
	}
	*bf = 0;
}

static void i2a(int num, char* bf)
{
	if (num < 0) {
		num = -num;
		*bf++ = '-';
	}
	ui2a((unsigned)num, 10, 0, bf);
}

static int a2d(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	return -1;
}

static char a2i(char ch, const char** src, int base, int* nump)
{
	const char* p = *src;
	int num = 0;
	int dgt = 0;

	while ((dgt = a2d(ch)) >= 0) {
		if (dgt > base)
			break;
		num = num * base + dgt;
		ch = *p++;
	}
	*src = p;
	*nump = num;
	return ch;
}

static void putchw(StIo* io, int n, char z, char* bf)
{
	char fc = z ? '0' : ' ';
	char ch;
	char* p = bf;

	while (*p++ && n > 0)
		n--;

	while (n-- > 0)
		io_putc(io, fc);

	while ((ch = *bf++))
		io_putc(io, ch);
}

void st_io_vprint(StIo* io, const char* fmt, va_list va)
{
	char bf[12] = {0};
	char ch = 0;
	char lz = 0;
	int w = 0;
	char lng = 0;

	while ((ch = *(fmt++))) {
		if (ch != '%') {
			io_putc(io, ch);
			continue;
		}

		ch = *(fmt++);
		if (ch == '0') {
			ch = *(fmt++);
			lz = 1;
		}

		if (ch >= '0' && ch <= '9')
			ch = a2i(ch, &fmt, 10, &w);

		if (ch == 'l') {
			ch = *(fmt++);
			lng = 1;
		}

		if (!ch)
			break;

		switch (ch) {
		case 'u':
			if (lng)
				ul2a(va_arg(va, unsigned long), 10, 0, bf);
			else
				ui2a(va_arg(va, unsigned), 10, 0, bf);
			putchw(io, w, lz, bf);
			break;
		case 'd':
			if (lng)
				li2a(va_arg(va, long), bf);
			else
				i2a(va_arg(va, int), bf);
			putchw(io, w, lz, bf);
			break;
		case 'x':
		case 'X':
			if (lng)
				ul2a(va_arg(va, unsigned long), 16, (ch == 'X'),
				     bf);
			else
				ui2a(va_arg(va, unsigned), 16, (ch == 'X'), bf);
			putchw(io, w, lz, bf);
			break;
		case 'c':
			io_putc(io, (char)(va_arg(va, int)));
			break;
		case 's':
			putchw(io, w, 0, va_arg(va, char*));
			break;
		case '%':
			io_putc(io, ch);
			break;
		default:
			VX_ASSERT(ch);
			break;
		}
	}
}

void st_io_print(StIo* io, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	io_vprint(io, fmt, va);
	va_end(va);
}
