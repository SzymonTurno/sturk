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
 * @file st/io/logger.h
 *
 * @brief Trace.
 */

#ifndef ST_IO_LOGGER_H
#define ST_IO_LOGGER_H

#include "st/io/buffer.h"

#ifndef ST_TRACKING

/**
 * @def ST_TRACKING
 *
 * @brief Traces enabled.
 */
#define ST_TRACKING 0

#endif /* ST_TRACKING */

/**
 * @def ST_TRACE(lvl, tag, ...)
 *
 * @brief Log to the streams that are attached to the logger.
 *
 * This is almost the same as st_trace(). The main difference is that it will
 * not compile if the ST_TRACKING is 0.
 *
 * @see st_trace()
 */
#define ST_TRACE(lvl, tag, ...)                                                \
	do {                                                                   \
		if (ST_TRACKING)                                               \
			st_trace(lvl, tag, __VA_ARGS__);                       \
	} while (0)

/**
 * @enum StTraceLvl
 *
 * @brief Trace level.
 */
enum StTraceLvl {
	ST_UNKNOWN = 0, /**< unknown trace level.    */
	ST_DEBUG,       /**< debug trace level.      */
	ST_INFO,        /**< info trace level.       */
	ST_WARNING,     /**< warning trace level.    */
	ST_ERROR,       /**< error trace level.      */
	ST_N_TRACE_LVLS /**< number of trace levels. */
};

/**
 * @fn void st_trace(enum StTraceLvl lvl, const char* tag, const char* format, ...)
 *
 * @brief Log to the file streams that are attached to the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in] tag The tag string.
 * @param[in] format The format string.
 * @param[in] ... The list of arguments.
 */
void st_trace(enum StTraceLvl lvl, const char* tag, const char* format, ...);

/**
 * @fn void st_logger_attach(enum StTraceLvl lvl, StIo* io)
 *
 * @brief Attach an io to the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in,out] io The io.
 */
void st_logger_attach(enum StTraceLvl lvl, StIo* io);

/**
 * @fn void st_logger_detach(enum StTraceLvl lvl, StIo* io)
 *
 * @brief Detach an io from the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in,out] io The io.
 */
void st_logger_detach(enum StTraceLvl lvl, StIo* io);

/**
 * @fn void st_logger_cleanup(void)
 *
 * @brief Detach all streams and free all the memory allocated by the logger.
 */
void st_logger_cleanup(void);

#endif /* ST_IO_LOGGER_H */
