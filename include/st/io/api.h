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
 * @file st/io/api.h
 *
 * @brief Input/output (IO) operations interface.
 */

#ifndef ST_IO_API_H
#define ST_IO_API_H

#include "st/arith.h"
#include <stdarg.h>

/**
 * @def ST_IO_EOF
 *
 * @brief End of file.
 */
#define ST_IO_EOF -1

/**
 * @struct StIo
 *
 * @brief Vtable for IO operations.
 *
 * Use this struct as the first member of a compound type and set its callbacks
 * to implement the IO interface.
 */
struct StIo {
	/**
	 * @var void (*putc_cb)(struct StIo*, char)
	 *
	 * @brief Callback for writing a single char.
	 */
	void (*putc_cb)(struct StIo*, char);

	/**
	 * @var char (*getc_cb)(struct StIo*)
	 *
	 * @brief Callback for reading a single char.
	 */
	char (*getc_cb)(struct StIo*);
};

/**
 * @fn void st_io_putc(struct StIo* io, char c)
 *
 * @brief Write a single char with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] c The char.
 */
void st_io_putc(struct StIo* io, char c);

/**
 * @fn char st_io_getc(struct StIo* io)
 *
 * @brief Read a single char with an IO interface.
 *
 * @param[in,out] io The IO interface.
 *
 * @return The char.
 */
char st_io_getc(struct StIo* io);

/**
 * @fn int st_io_vprint(struct StIo* io, const char* fmt, va_list va)
 *
 * @brief Convert data from a va_list and write it with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] fmt The format string.
 * @param[in] va The va_list.
 */
int st_io_vprint(struct StIo* io, const char* fmt, va_list va);

/**
 * @fn int st_io_print(struct StIo* io, const char* fmt, ...)
 *
 * @brief Convert data from "..." and write it with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] fmt The format string.
 * @param[in] ... The data.
 */
int st_io_print(struct StIo* io, const char* fmt, ...);

/**
 * @fn char* st_io_fgets(char* str, int size, struct StIo* io)
 *
 * @brief Read a number of characters with an IO interface to a string.
 *
 * @param[in,out] str The destination string.
 * @param[in] size The number of characters to read.
 * @param[in] io The IO interface.
 *
 * @return io The destination string.
 */
char* st_io_fgets(char* str, int size, struct StIo* io);

#endif /* ST_IO_API_H */
