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
 * @file st/os/sys.h
 *
 * @brief System procedures.
 */

#ifndef ST_SYS_H
#define ST_SYS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @struct StException
 *
 * @brief Exception.
 */
struct StException {
	/**
	 * @var const char* reason
	 *
	 * @brief The reason that caused the exception.
	 */
	const char* reason;
};

/** Exception: "null_param". */
static const struct StException st_except_null_param /**/
	= {"Null param."};

/** Exception: "alloc_fail". */
static const struct StException st_except_alloc_fail /**/
	= {"Memory allocation failed."};

/** Exception: "sem_fail". */
static const struct StException st_except_sem_fail /**/
	= {"Semaphore failure."};

/** Exception: "mutex_fail". */
static const struct StException st_except_mutex_fail /**/
	= {"Mutex failure."};

/** Exception: "not_supported". */
static const struct StException st_except_not_supported /**/
	= {"Not supported."};

/** Exception: "sanity_fail". */
static const struct StException st_except_sanity_fail /**/
	= {"Sanity check failed."};

/**
 * @fn void st_strprint(char* buff, const char* fmt, ...)
 *
 * @see snprintf()
 */
void st_strprint(char* buff, const char* fmt, ...);

/**
 * @fn void st_except(const char* reason, const char* file, int line)
 *
 * @brief Log error reason and call exit(EXIT_FAILURE).
 *
 * @param[in] reason The reason.
 * @param[in] file The name of the source file.
 * @param[in] line The line number.
 */
void st_except(const char* reason, const char* file, int line);

/**
 * @def ST_EXCEPT(e)
 *
 * @see st_except()
 */
#define ST_EXCEPT(e) st_except(st_except_##e.reason, __FILE__, __LINE__)

#endif /* ST_SYS_H */
