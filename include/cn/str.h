/**
 * @file cn/str.h
 * @brief Header file for string data structures.
 *
 * *** todo ***.
 */

#ifndef CN_STR_H
#define CN_STR_H

#include "cn/cirq.h"
#include "cn/dict.h"
#include "cn/list.h"

/**
 * @struct CnStrlist
 * @brief *** todo ***.
 */
CN_LIST(struct CnStrlist, char*); /**< CN_LIST(struct CnStrlist, char*) */

/**
 * @struct CnStrq
 * @brief *** todo ***.
 */
CN_CIRQ(struct CnStrq, char*); /**< CN_CIRQ(struct CnStrq, char*) */

/* clang-format off */
/**
 * @struct CnStrbag
 * @brief *** todo ***.
 */
CN_DICT(struct CnStrbag, union {int n; void* align;}); /**< CN_DICT(struct CnStrbag, union {int n; void* align;}) */
/* clang-format on */

/**
 * @fn char* cn_newstr(const char* str)
 * @brief *** todo ***.
 * @param[in] str Input.
 * @return *** todo ***.
 */
char* cn_newstr(const char* str);

/**
 * @fn struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str)
 * @brief *** todo ***.
 * @param[in,out] list Input/output.
 * @param[in] str Input.
 * @return *** todo ***.
 */
struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str);

/**
 * @fn char* cn_strlist_rem(struct CnStrlist** listp)
 * @brief *** todo ***.
 * @param[in,out] listp Input/output.
 * @return *** todo ***.
 */
char* cn_strlist_rem(struct CnStrlist** listp);

/**
 * @fn struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str)
 * @brief *** todo ***.
 * @param[in,out] q Input/output.
 * @param[in] str Input.
 * @return *** todo ***.
 */
struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str);

/**
 * @fn char* cn_strq_rem(struct CnStrq** qp)
 * @brief *** todo ***.
 * @param[in,out] qp Input/output.
 * @return *** todo ***.
 */
char* cn_strq_rem(struct CnStrq** qp);

/**
 * @fn struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str)
 * @brief *** todo ***.
 * @param[in,out] bag Input/output.
 * @param[in] str Input.
 * @return *** todo ***.
 */
struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str);

/**
 * @fn struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str)
 * @brief *** todo ***.
 * @param[in,out] bag Input/output.
 * @param[in] str Input.
 * @return *** todo ***.
 */
struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str);

/**
 * @fn int cn_strbag_count(const struct CnStrbag* bag)
 * @brief *** todo ***.
 * @param[in] bag Input.
 * @return *** todo ***.
 */
int cn_strbag_count(const struct CnStrbag* bag);

/**
 * @fn void cn_strbag_destroy(struct CnStrbag* bag)
 * @brief *** todo ***.
 * @param[in,out] bag Input/output.
 */
void cn_strbag_destroy(struct CnStrbag* bag);

#endif /* CN_STR_H */
