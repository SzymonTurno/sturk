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
 * @file st/logger/streambag.h
 *
 * @brief Bag of streams.
 */

#ifndef ST_LOGGER_STREAMBAG_H
#define ST_LOGGER_STREAMBAG_H

#include "st/os/fstream.h"

/**
 * @var typedef struct StStreamBag StStreamBag
 *
 * @brief A bag of streams.
 */
typedef struct StStreamBag StStreamBag;

/**
 * @fn StStreamBag* st_streambag_create(void)
 *
 * @brief Create a new empty bag of streams.
 *
 * @return An empty bag of streams.
 */
StStreamBag* st_streambag_create(void);

/**
 * @fn void st_streambag_destroy(StStreamBag* bag)
 *
 * @brief Destroy a bag of streams
 *
 * @param[in,out] bag The bag.
 */
void st_streambag_destroy(StStreamBag* bag);

/**
 * @fn void st_streambag_ins(StStreamBag* bag, StFstream* stream)
 *
 * @brief Insert a stream into a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] stream The stream.
 */
void st_streambag_ins(StStreamBag* bag, StFstream* stream);

/**
 * @fn void st_streambag_rem(StStreamBag* bag, StFstream* stream)
 *
 * @brief Remove a stream from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] stream The stream.
 */
void st_streambag_rem(StStreamBag* bag, StFstream* stream);

/**
 * @fn void st_streambag_vprint(StStreamBag* bag, const char* format, va_list vlist)
 *
 * @brief Print to all streams from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] format The format string.
 * @param[in] vlist The list of arguments.
 */
void st_streambag_vprint(StStreamBag* bag, const char* format, va_list vlist);

/**
 * @fn int st_streambag_count(const StStreamBag* bag)
 *
 * @brief Get the number of streams in a bag.
 *
 * @param[in] bag The bag.
 *
 * @return The number of streams.
 */
int st_streambag_count(const StStreamBag* bag);

#endif /* ST_LOGGER_STREAMBAG_H */
