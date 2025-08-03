/**
 * @file cn/str.h
 *
 * @brief String utilities.
 */

#ifndef CN_STR_H
#define CN_STR_H

#include "cn/cirq.h"
#include "cn/dict.h"
#include "cn/list.h"

/**
 * @def CN_LIST(struct CnStrlist, char*)
 *
 * @struct CnStrlist
 *
 * @brief List of strings.
 */
CN_LIST(struct CnStrlist, char*);
/**< This is a macro definition of the struct CnStrlist type. */

/**
 * @def CN_CIRQ(struct CnStrq, char*)
 *
 * @struct CnStrq
 *
 * @brief Queue of strings.
 */
CN_CIRQ(struct CnStrq, char*);
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
