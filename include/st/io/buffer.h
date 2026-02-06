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
 * @brief Memory buffer stream.
 */

#ifndef ST_IO_BUFFER_H
#define ST_IO_BUFFER_H

#include "st/io/api.h"

/**
 * @struct StIoBuffer
 *
 * @brief Interface for a memory buffer stream.
 */
struct StIoBuffer {
	/**
	 * @var struct StIo _vt
	 *
	 * @brief IO interface.
	 */
	struct StIo _vt;

	/**
	 * @var char* cp;
	 *
	 * @brief Stream pointer.
	 */
	char* cp;
};

/**
 * @fn struct StIo* st_iobuffer_init(struct StIoBuffer* io_buff)
 *
 * @brief Initialize a memory buffer stream.
 *
 * In order to use the memory buffer stream interface: initialize the interface
 * and assign the front of the buffer to its `cp` member.
 *
 * ```
 *     struct StIoBuffer io_buff = {.cp = buff};
 *     struct StIo* io = st_iobuffer_init(&io_buff);
 * ```
 *
 * @param[in,out] io_buff The memory buffer stream interface.
 *
 * @return A pointer to the IO interface.
 */
struct StIo* st_iobuffer_init(struct StIoBuffer* io_buff);

/**
 * @def st_iocontig_calclen(n_bytes)
 *
 * @brief Calculate the length of `struct StIo` array.
 *
 * Initialize the array with st_iobuffer_init() to use it as a memory buffer
 * stream of the size @a nbytes.
 *
 * ```
 *     struct StIo io[st_iobuffer_init(N)];
 *
 *     st_iobuffer_init((struct StIoBuffer*)io);
 * ```
 *
 * @param[in] n_bytes The number of bytes for free space.
 *
 * @return The number of elements in a `struct StIo` array.
 *
 * @see st_iobuffer_init()
 */
#define st_iocontig_calclen(n_bytes)                                           \
	(((n_bytes) + sizeof(struct StIoBuffer) - 1) / sizeof(struct StIo) + 1)

/**
 * @fn static inline char* st_iocontig_front(struct StIo* io)
 *
 * @brief Get the front of a memory buffer.
 *
 * @param[in,out] io IO interface.
 *
 * @return The front of a memory buffer.
 */
static inline char* st_iocontig_front(struct StIo* io)
{
	return (char*)&((struct StIoBuffer*)io)[1];
}

#endif /* ST_IO_BUFFER_H */
