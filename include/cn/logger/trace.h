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
 * @file cn/logger/trace.h
 *
 * @brief Trace.
 */

#ifndef CN_LOGGER_TRACE_H
#define CN_LOGGER_TRACE_H

#include "cn/os/fstream.h"

#ifndef CN_LOGGER_EN

/**
 * @def CN_LOGGER_EN
 *
 * @brief Logger enabled.
 */
#define CN_LOGGER_EN 0

#endif /* CN_LOGGER_EN */

/**
 * @def CN_TRACE(lvl, tag, ...)
 *
 * @brief Log to the streams that are attached to the logger.
 *
 * This is almost the same as cn_trace(). The main difference is that it will
 * not compile if the CN_LOGGER_EN is 0.
 *
 * @see cn_trace()
 */
#define CN_TRACE(lvl, tag, ...)                                                \
	do {                                                                   \
		if (CN_LOGGER_EN)                                              \
			cn_trace(lvl, tag, __VA_ARGS__);                       \
	} while (0)

/**
 * @enum CnTraceLvl
 *
 * @brief Trace level.
 */
enum CnTraceLvl {
	CN_UNKNOWN = 0, /**< unknown trace level.    */
	CN_DEBUG,       /**< debug trace level.      */
	CN_INFO,        /**< info trace level.       */
	CN_WARNING,     /**< warning trace level.    */
	CN_ERROR,       /**< error trace level.      */
	CN_N_TRACE_LVLS /**< number of trace levels. */
};

/**
 * @fn void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...)
 *
 * @brief Log to the file streams that are attached to the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in] tag The tag string.
 * @param[in] format The format string.
 * @param[in] ... The list of arguments.
 */
void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...);

/**
 * @fn void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream)
 *
 * @brief Attach a stream to the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in,out] stream The stream.
 */
void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream)
 *
 * @brief Detach a stream from the logger.
 *
 * @param[in] lvl The trace level.
 * @param[in,out] stream The stream.
 */
void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_cleanup(void)
 *
 * @brief Detach all streams and free all the memory allocated by the logger.
 */
void cn_logger_cleanup(void);

#endif /* CN_LOGGER_TRACE_H */
