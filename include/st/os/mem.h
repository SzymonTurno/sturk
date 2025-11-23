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
 * @file st/os/mem.h
 *
 * @brief System memory allocator.
 */

#ifndef ST_OS_MEM_H
#define ST_OS_MEM_H

#include <stddef.h>

/**
 * @def st_alloc(size)
 *
 * @see st_mem_alloc()
 */
#define st_alloc(size) st_mem_alloc((size), __FILE__, __LINE__)

/**
 * @def st_free(size)
 *
 * @see st_mem_free()
 */
#define st_free(ptr) st_mem_free((ptr), __FILE__, __LINE__)

/* @cond */
#define ST__NEW(type, n, ...) ((type*)st_alloc(sizeof(type) * n))
/* @endcond */

/**
 * @def ST_NEW(...)
 *
 * @brief Allocate memory for a data type.
 *
 * A call ST_NEW(type, n) will alocate contiguous memory region of the length
 * that is equal to the multiple of @a n and the size of @a type. The @a n
 * argument is optional and by default it equals 1.
 */
#define ST_NEW(...) ST__NEW(__VA_ARGS__, 1, )

/**
 * @fn st_mem_alloc(size_t size, const char* file, int line)
 *
 * @brief Allocate a contiguous memory region and optionally log a message on failure.
 *
 * @param[in] size The number of bytes to allocate.
 * @param[in] file The name of the source file.
 * @param[in] line The line number.
 *
 * @return The pointer to the allocated memory region.
 */
void* st_mem_alloc(size_t size, const char* file, int line);

/**
 * @fn void st_mem_free(void* ptr, const char* file, int line)
 *
 * @brief Free a contiguous memory region and optionally log a message on failure.
 *
 * @param[in] ptr The pointer to the memory region.
 * @param[in] file The name of the source file.
 * @param[in] line The line number.
 */
void st_mem_free(void* ptr, const char* file, int line);

/*
 * @fn void st_mem_expand(void* buff, size_t size)
 *
 * @brief Add a buffer to the free memory.
 *
 * @param[in] buff The buffer.
 * @param[in] size The size of the buffer in bytes.
 */
/* void st_mem_expand(void* buff, size_t size); */

/**
 * @fn void st_mem_cleanup(void)
 *
 * @brief If supported, free all the memory allocated with the system allocator.
 */
void st_mem_cleanup(void);

#endif /* ST_OS_MEM_H */
