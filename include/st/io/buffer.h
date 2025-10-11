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
 * @brief Input/output operations interface.
 */

#ifndef ST_IO_BUFFER_H
#define ST_IO_BUFFER_H

#include "st/arith.h"
#include <stdarg.h>

/**
 * @def ST_IO_EOF
 *
 * @brief End of file.
 */
#define ST_IO_EOF -1

/**
 * @def st_iobuffer_calclen(bytes)
 *
 * @brief Calculate the length of an iobuffer array.
 *
 * @param[in] bytes The number of bytes of the required free space.
 */
#define st_iobuffer_calclen(bytes)                                             \
	(3 + (sizeof(char*) + (bytes) - 1) / sizeof(StAlign))

/**
 * @var typedef StAlign StIoBuffer
 *
 * @brief The IO buffer.
 */
typedef StAlign StIoBuffer;

/**
 * @var typedef union StIo StIo
 *
 * @brief The IO interface.
 */
typedef union StIo StIo;

/**
 * @struct StIoVt
 *
 * @brief Vtable for IO operations.
 */
struct StIoVt {
	/**
	 * @var void (*putc_cb)(void*, char)
	 *
	 * @brief Callback for writing a single char.
	 */
	void (*putc_cb)(void*, char);

	/**
	 * @var char (*getc_cb)(void*)
	 *
	 * @brief Callback for reading a single char.
	 */
	char (*getc_cb)(void*);
};

/**
 * @fn StIo* st_io_init(StIoBuffer* buff)
 *
 * @brief Initialize an IO interface from a memory buffer.
 *
 * @param[in,out] buff The memory buffer.
 *
 * @return The pointer to the IO interface.
 */
StIo* st_io_init(StIoBuffer* buff);

/**
 * @fn void st_io_setp(StIo* io, void* p)
 *
 * @brief Set the pointer that is passed to the IO operations.
 *
 * @param[in,out] io The IO interface.
 * @param[in] p The pointer.
 */
void st_io_setp(StIo* io, void* p);

/**
 * @fn void st_io_setvp(StIo* io, const struct StIoVt* vp)
 *
 * @brief Set the API for IO operations.
 *
 * @param[in,out] io The IO interface.
 * @param[in] vp The pointer to the vtable.
 */
void st_io_setvp(StIo* io, const struct StIoVt* vp);

/**
 * @fn void st_io_putc(StIo* io, char c)
 *
 * @brief Write a single char with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] c The char.
 */
void st_io_putc(StIo* io, char c);

/**
 * @fn char st_io_getc(StIo* io)
 *
 * @brief Read a single char with an IO interface.
 *
 * @param[in,out] io The IO interface.
 *
 * @return The char.
 */
char st_io_getc(StIo* io);

/**
 * @fn void st_io_vprint(StIo* io, const char* fmt, va_list va)
 *
 * @brief Convert data from a va_list and write it with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] fmt The format string.
 * @param[in] va The va_list.
 */
void st_io_vprint(StIo* io, const char* fmt, va_list va);

/**
 * @fn void st_io_print(StIo* io, const char* fmt, ...)
 *
 * @brief Convert data from "..." and write it with an IO interface.
 *
 * @param[in,out] io The IO interface.
 * @param[in] fmt The format string.
 * @param[in] ... The data.
 */
void st_io_print(StIo* io, const char* fmt, ...);

/**
 * @fn char* st_io_fgets(char* str, int size, StIo* io)
 *
 * @brief Read a number of characters with an IO interface to a string.
 *
 * @param[in,out] str The destination string.
 * @param[in] size The number of characters to read.
 * @param[in] io The IO interface.
 *
 * @return io The destination string.
 */
char* st_io_fgets(char* str, int size, StIo* io);

#endif /* ST_IO_BUFFER_H */
