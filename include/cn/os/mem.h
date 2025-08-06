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
 * @file cn/os/mem.h
 *
 * @brief System memory allocator.
 */

#ifndef CN_OS_MEM_H
#define CN_OS_MEM_H

#include <stddef.h>

/* @cond */
#define CN__NEW(type, n, ...) ((type*)cn_malloc(sizeof(type) * n))
/* @endcond */

/**
 * @def CN_NEW(...)
 *
 * @brief Allocate memory for a data type.
 *
 * A call CN_NEW(type, n) will alocate contiguous memory region of the length
 * that is equal to the multiple of @a n and the size of @a type. The @a n
 * argument is optional and by default it equals 1.
 */
#define CN_NEW(...) CN__NEW(__VA_ARGS__, 1, )

/**
 * @fn void* cn_malloc(size_t size)
 *
 * @see malloc()
 */
void* cn_malloc(size_t size);

/**
 * @fn void cn_free(void* ptr)
 *
 * @see free()
 */
void cn_free(void* ptr);

#endif /* CN_OS_MEM_H */
