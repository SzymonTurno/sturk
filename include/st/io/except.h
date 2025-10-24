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
 * @file st/io/except.h
 *
 * @brief Exceptions.
 */

#ifndef ST_IO_EXCEPT_H
#define ST_IO_EXCEPT_H

#include "st/io/logger.h"
#include "st/os/sys.h"

/**
 * @def ST_RAISE(lvl, e)
 *
 * @brief Raise an exception.
 *
 * @param[in] lvl The exception level (WARNING or ERROR).
 * @param[in] e The exception.
 */
#define ST_RAISE(lvl, e)                                                       \
	do {                                                                   \
		enum StTraceLvl _lvl = (lvl);                                  \
                                                                               \
		if (ST_ERROR == _lvl) {                                        \
			ST_TRACE(                                              \
				ST_ERROR, NULL, "%s:%d: %s", __FILE__,         \
				__LINE__, st_except_##e.reason);               \
			ST_EXCEPT(e);                                          \
		} else if (ST_WARNING == _lvl) {                               \
			ST_TRACE(                                              \
				ST_WARNING, NULL, "%s:%d: %s", __FILE__,       \
				__LINE__, st_except_##e.reason);               \
		} else {                                                       \
			ST_TRACE(                                              \
				ST_ERROR, NULL, "%s:%d: %s", __FILE__,         \
				__LINE__, st_except_not_supported.reason);     \
		}                                                              \
	} while (0)

/**
 * @def ST_ENSURE(cond, lvl, e)
 *
 * @brief Raise an exception if the condition is not met.
 *
 * @param[in] cond The condition.
 * @param[in] lvl The exception level: WARNING or ERROR.
 * @param[in] e The exception.
 */
#define ST_ENSURE(cond, lvl, e)                                                \
	do {                                                                   \
		if (!(cond))                                                   \
			ST_RAISE(lvl, e);                                      \
	} while (0)

/**
 * @def ST_ENSURE_MEM(lvl, ptr)
 *
 * @brief Raise an exception and return NULL if the pointer is NULL.
 *
 * @param[in] ptr The pointer.
 * @param[in] lvl Exception level.
 */
#define ST_ENSURE_MEM(ptr, lvl)                                                \
	do {                                                                   \
		if ((ptr) == NULL) {                                           \
			ST_RAISE(lvl, null_param);                             \
			return NULL;                                           \
		}                                                              \
	} while (0)

#endif /* ST_IO_EXCEPT_H */
