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
 * @file st/str.h
 *
 * @brief String utilities.
 */

#ifndef ST_STR_H
#define ST_STR_H

#include "st/dict.h"
#include "vertegs/cirq.h"
#include "vertegs/list.h"

/**
 * @def VX_LIST(struct StStrList, char*)
 *
 * @struct StStrList
 *
 * @brief List of strings.
 */
VX_LIST(struct StStrList, char*);
/**< This is a macro definition of a type for a list of strings. */

/**
 * @def VX_CIRQ(struct StStrQ, char*)
 *
 * @struct StStrQ
 *
 * @brief Queue of strings.
 */
VX_CIRQ(struct StStrQ, char*);
/**< This is a macro definition of a type for a queue of strings. */

/* clang-format off */
/**
 * @def ST_DICT(struct StStrBag, union {int n; void* align;})
 *
 * @struct StStrBag
 *
 * @brief Bag of strings.
 */
ST_DICT(struct StStrBag, union {int n; void* align;});
/**< This is a macro definition of a type for a bag of strings. */
/* clang-format on */

/**
 * @fn size_t st_strlen(const char* str)
 *
 * @see strlen()
 */
size_t st_strlen(const char* str);

/**
 * @fn char* st_strcpy(char* dest, const char* src)
 *
 * @see strcpy()
 */
char* st_strcpy(char* dest, const char* src);

/**
 * @fn int st_strcmp(const char* s1, const char* s2)
 *
 * @see strcmp()
 */
int st_strcmp(const char* s1, const char* s2);

/**
 * @fn char* st_strdup(const char* str)
 *
 * @see strdup()
 */
char* st_strdup(const char* str);

/**
 * @fn struct StStrList* st_strlist_ins(struct StStrList* list, char* str)
 *
 * @brief Insert a string into a list.
 *
 * @param[in,out] list The head of the list.
 * @param[in] str The string.
 *
 * @return The new head of the list.
 */
struct StStrList* st_strlist_ins(struct StStrList* list, char* str);

/**
 * @fn char* st_strlist_rem(struct StStrList** listp)
 *
 * @brief Remove a string from a list.
 *
 * @param[in,out] listp A double pointer to the head fo the list.
 *
 * @return The removed string.
 */
char* st_strlist_rem(struct StStrList** listp);

/**
 * @fn struct StStrQ* st_strq_ins(struct StStrQ* q, char* str)
 *
 * @brief Insert a string into a queue.
 *
 * @param[in,out] q The head of the queue.
 * @param[in] str The string.
 *
 * @return The new head of the queue.
 */
struct StStrQ* st_strq_ins(struct StStrQ* q, char* str);

/**
 * @fn char* st_strq_rem(struct StStrQ** qp)
 *
 * @brief Remove a string from a queue.
 *
 * @param[in,out] qp A double pointer to the head of the queue.
 *
 * @return The removed string.
 */
char* st_strq_rem(struct StStrQ** qp);

/**
 * @fn struct StStrBag* st_strbag_ins(struct StStrBag* bag, const char* str)
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
struct StStrBag* st_strbag_ins(struct StStrBag* bag, const char* str);

/**
 * @fn struct StStrBag* st_strbag_rem(struct StStrBag* bag, const char* str)
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
struct StStrBag* st_strbag_rem(struct StStrBag* bag, const char* str);

/**
 * @fn int st_strbag_count(const struct StStrBag* bag)
 *
 * @brief For a bag entry, get the occurence count of its string.
 *
 * @param[in] bag The bag entry.
 *
 * @return The occurence count.
 */
int st_strbag_count(const struct StStrBag* bag);

/**
 * @fn void st_strbag_destroy(struct StStrBag* bag)
 *
 * @brief Destroy the whole bag of strings.
 *
 * @param[in,out] bag Any entry from the bag.
 */
void st_strbag_destroy(struct StStrBag* bag);

#endif /* ST_STR_H */
