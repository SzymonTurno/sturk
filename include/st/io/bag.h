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
 * @file st/io/bag.h
 *
 * @brief Bag of IO instances.
 */

#ifndef ST_IO_BAG_H
#define ST_IO_BAG_H

#include "st/io/api.h"

/**
 * @var typedef struct StIoBag StIoBag
 *
 * @brief A bag of IO instances.
 */
typedef struct StIoBag StIoBag;

/**
 * @fn StIoBag* st_iobag_create(void)
 *
 * @brief Create a bag of IO interface instances.
 *
 * @return The pointer to the bag.
 */
StIoBag* st_iobag_create(void);

/**
 * @fn void st_iobag_destroy(StIoBag* bag)
 *
 * @brief Destroy a bag of IO interface instances.
 *
 * @param[in,out] bag The bag.
 */
void st_iobag_destroy(StIoBag* bag);

/**
 * @fn void st_iobag_ins(StIoBag* bag, struct StIo* io)
 *
 * @brief Insert an IO interface into a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in,out] io The IO interface.
 */
void st_iobag_ins(StIoBag* bag, struct StIo* io);

/**
 * @fn void st_iobag_rem(StIoBag* bag, struct StIo* io)
 *
 * @brief Remove an IO interface from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in,out] io The IO interface.
 */
void st_iobag_rem(StIoBag* bag, struct StIo* io);

/**
 * @fn void st_iobag_vprint(StIoBag* bag, const char* fmt, va_list va)
 *
 * @brief Convert data from a va_list and write it with all IO interfaces from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] fmt The format string.
 * @param[in] va The va_list.
 */
void st_iobag_vprint(StIoBag* bag, const char* fmt, va_list va);

/**
 * @fn size_t st_iobag_count(const StIoBag* bag)
 *
 * @brief Count IO interface instances in a bag.
 *
 * @param[in] bag The bag.
 *
 * @return The number of instances.
 */
size_t st_iobag_count(const StIoBag* bag);

#endif /* ST_IO_BAG_H */
