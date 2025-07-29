/**
 * @file cn/cirq.h
 *
 * @brief Doubly linked circular list.
 *
 * This header file provides data types, functions and macros that define and
 * operate on doubly linked circular lists.
 *
 * @anchor r-cirq_gloss
 * <h4> Glossary </h4>
 * | Term | Description                 |
 * | ---- | --------------------------- |
 * | cirq | doubly linked circular list |
 *
 * <h4> Usage </h4>
 * @code
 * typedef SomeData MyData;
 *
 * CN_CIRQ(struct MyCirq, MyData);
 *
 * void init_and_push(struct MyCirq** headp, struct MyCirq* entry, MyData data)
 * {
 *     *cn_cirq_data(entry) = data;
 *     *headp = cn_cirq_ins(*headp, entry);
 * }
 * @endcode
 */

#ifndef CN_CIRQ_H
#define CN_CIRQ_H

#include "cn/arith.h"
#include "cn/logger/except.h"

#ifdef __STRICT_ANSI__

#define cn_cirq_cast(cirq) (&(cirq)->node)

#define cn_cirq_data(cirq) (&(cirq)->data)

#define cn_cirq_from(ptr, type) ((type*)(ptr))

#define _CN_CIRQ_INS(cirq, entry, pos, ...)                                    \
	((void*)cn_binode_ins((struct CnBinode*)(cirq), &(entry)->node, pos))

#define _CN_CIRQ_REM(cirqp, pos, ...)                                          \
	((void*)cn_binode_rem((struct CnBinode**)(cirqp), pos))

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def cn_cirq_cast(cirq)
 *
 * @brief Get the pointer to the CnBinode member of the @ref r-cirq_gloss "cirq" entry.
 *
 * @param[in] cirq The @ref r-cirq_gloss "cirq" entry pointer.
 *
 * @return A pointer to the CnBinode.
 *
 * @note Remove __STRICT_ANSI__ to enable type check for @a cirq.
 */
#define cn_cirq_cast(cirq)                                                     \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
                                                                               \
		CN_ENSURE(_cirq, CN_ERROR, null_param);                        \
		&_cirq->node;                                                  \
	})

/**
 * @def cn_cirq_data(cirq)
 *
 * @brief Get the pointer to the data member of the @ref r-cirq_gloss "cirq" entry.
 *
 * @param[in] cirq The @ref r-cirq_gloss "cirq" entry pointer.
 *
 * @return A pointer to the data.
 *
 * @note Remove __STRICT_ANSI__ to enable type check for @a cirq.
 */
#define cn_cirq_data(cirq)                                                     \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
                                                                               \
		CN_ENSURE(_cirq, CN_ERROR, null_param);                        \
		&_cirq->data;                                                  \
	})

/**
 * @def cn_cirq_from(ptr, type)
 *
 * @brief Cast a CnBinode member out to the containing @ref r-cirq_gloss "cirq" entry.
 *
 * @param[in] ptr Input.
 * @param[in] type Input.
 *
 * @return A pointer to the @ref r-cirq_gloss "cirq" entry.
 */
#define cn_cirq_from(ptr, type) cn_container_of(ptr, type, node)

/* @cond */
#define _CN_CIRQ_INS(cirq, entry, pos, ...)                                    \
	({                                                                     \
		__typeof__(entry) _cirq = (cirq);                              \
		__typeof__(entry) _entry = (entry);                            \
                                                                               \
		cn_cirq_from(                                                  \
			cn_binode_ins(                                         \
				_cirq ? &_cirq->node : NULL,                   \
				_entry ? &_entry->node : NULL, (pos)),         \
			__typeof__(*entry));                                   \
	})

#define _CN_CIRQ_REM(cirqp, pos, ...)                                          \
	({                                                                     \
		__typeof__(cirqp) _cirqp = (cirqp);                            \
		struct CnBinode* _node =                                       \
			(_cirqp && *_cirqp) ? &(*_cirqp)->node : NULL;         \
		__typeof__(*cirqp) _ret = cn_cirq_from(                        \
			cn_binode_rem(&_node, (pos)), __typeof__(**_cirqp));   \
                                                                               \
		*_cirqp = cn_cirq_from(_node, __typeof__(**_cirqp));           \
		_ret;                                                          \
	})
/* @endcond */

#endif /* __STRICT_ANSI__ */

/**
 * @def cn_cirq_ins(cirq, ...)
 *
 * @brief *** todo ***.
 *
 * @param[in,out] cirq Input/output.
 * @param[in,out] ... (1) entry input/output; (2) pos (optional) input, -1 by default.
 *
 * @return *** todo ***.
 *
 * No type check for @a cirq with __STRICT_ANSI__ build.
 */
#define cn_cirq_ins(cirq, ...) _CN_CIRQ_INS((cirq), __VA_ARGS__, -1, )

/**
 * @def cn_cirq_rem(...)
 * @brief *** todo ***.
 * @param[in,out] ... (1) cirqp input/output; (2) pos (optional) input, 0 by default.
 * @returns *** todo ***.
 *
 * No type check for \a cirqp with __STRICT_ANSI__ build.
 */
#define cn_cirq_rem(...) _CN_CIRQ_REM(__VA_ARGS__, 0, )

/**
 * @def CN_CIRQ(name, type)
 *
 * @brief Define the @ref r-cirq_gloss "cirq".
 *
 * @param[in] name The name of the type used for @ref r-cirq_gloss "cirq".
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a @ref r-cirq_gloss "cirq" entry that holds the data of the type
 * @a type.
 */
#define CN_CIRQ(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnBinode node;                                          \
		type data;                                                     \
	}

/**
 * @struct CnBinode
 * @brief *** todo ***.
 *
 * Members:
 * - next,
 * - prev.
 */
struct CnBinode {
	/**
	 * @var struct CnBinode* next
	 * @brief *** todo ***.
	 */
	struct CnBinode* next;

	/**
	 * @var struct CnBinode* prev
	 * @brief *** todo ***.
	 */
	struct CnBinode* prev;
};

/**
 * @fn struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos)
 * @brief *** todo ***.
 * @param[in,out] node Input/output.
 * @param[in] pos Input.
 * @returns *** todo ***.
 */
struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos);

/**
 * @fn struct CnBinode* cn_binode_ins(struct CnBinode* root, struct CnBinode* entry, int pos)
 * @brief *** todo ***.
 * @param[in,out] root Input/output.
 * @param[in,out] entry Input/output.
 * @param[in] pos Input.
 * @returns *** todo ***.
 */
struct CnBinode*
cn_binode_ins(struct CnBinode* root, struct CnBinode* entry, int pos);

/**
 * @fn struct CnBinode* cn_binode_rem(struct CnBinode** rootp, int pos)
 * @brief *** todo ***.
 * @param[in,out] rootp Input/output.
 * @param[in] pos Input.
 * @returns *** todo ***.
 */
struct CnBinode* cn_binode_rem(struct CnBinode** rootp, int pos);

#endif /* CN_CIRQ_H */
