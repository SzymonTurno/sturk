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
 * @file cn/str.h
 *
 * @brief String utilities.
 */

#ifndef CN_STR_H
#define CN_STR_H

#include "cn/dict.h"
#include "vx/cirq.h"
#include "vx/list.h"

/**
 * @def CN_LIST(struct CnStrlist, char*)
 *
 * @struct CnStrlist
 *
 * @brief List of strings.
 */
VX_LIST(struct CnStrlist, char*);
/**< This is a macro definition of the struct CnStrlist type. */

/**
 * @def CN_CIRQ(struct CnStrq, char*)
 *
 * @struct CnStrq
 *
 * @brief Queue of strings.
 */
VX_CIRQ(struct CnStrq, char*);
/**< This is a macro definition of the struct CnStrq type. */

/* clang-format off */
/**
 * @def CN_DICT(struct CnStrbag, union {int n; void* align;})
 *
 * @struct CnStrbag
 *
 * @brief Bag of strings.
 */
CN_DICT(struct CnStrbag, union {int n; void* align;});
/**< This is a macro definition of the struct CnStrbag type. */
/* clang-format on */

/**
 * @fn char* cn_newstr(const char* str)
 *
 * @see strdup()
 */
char* cn_newstr(const char* str);

/**
 * @fn struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str)
 *
 * @brief Insert a string into a list.
 *
 * @param[in,out] list The head of the list.
 * @param[in] str The string.
 *
 * @return The new head of the list.
 */
struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str);

/**
 * @fn char* cn_strlist_rem(struct CnStrlist** listp)
 *
 * @brief Remove a string from a list.
 *
 * @param[in,out] listp A double pointer to the head fo the list.
 *
 * @return The removed string.
 */
char* cn_strlist_rem(struct CnStrlist** listp);

/**
 * @fn struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str)
 *
 * @brief Insert a string into a queue.
 *
 * @param[in,out] q The head of the queue.
 * @param[in] str The string.
 *
 * @return The new head of the queue.
 */
struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str);

/**
 * @fn char* cn_strq_rem(struct CnStrq** qp)
 *
 * @brief Remove a string from a queue.
 *
 * @param[in,out] qp A double pointer to the head of the queue.
 *
 * @return The removed string.
 */
char* cn_strq_rem(struct CnStrq** qp);

/**
 * @fn struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str)
 *
 * @brief Insert a string into a bag.
 *
 * @param[in,out] bag The root of the bag.
 * @param[in] str The string.
 *
 * This will increment the occurence count of the string.
 *
 * @return The new root of the bag.
 */
struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str);

/**
 * @fn struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str)
 *
 * @brief Remove a string from a bag.
 *
 * @param[in,out] bag The root of the bag.
 * @param[in] str The string.
 *
 * This will decrement the occurence count of the string. Negative counts are
 * supported.
 *
 * @return The new root of the bag.
 */
struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str);

/**
 * @fn int cn_strbag_count(const struct CnStrbag* bag)
 *
 * @brief For a bag entry, get the occurence count of its string.
 *
 * @param[in] bag The bag entry.
 *
 * @return The occurence count.
 */
int cn_strbag_count(const struct CnStrbag* bag);

/**
 * @fn void cn_strbag_destroy(struct CnStrbag* bag)
 *
 * @brief Destroy the whole bag of strings.
 *
 * @param[in,out] bag Any entry from the bag.
 */
void cn_strbag_destroy(struct CnStrbag* bag);

#endif /* CN_STR_H */
