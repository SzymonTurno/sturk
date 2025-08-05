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
 * @file cn/cirq.h
 *
 * @brief Doubly linked circular list.
 *
 * This header file provides data types, functions and macros that define and
 * operate on doubly linked circular lists.
 *
 *
 * Features
 * --------
 *
 * - Intrusive.
 *
 *
 * Glossary
 * --------
 *
 * | Term | Description                 |
 * | ---- | --------------------------- |
 * | cirq | doubly linked circular list |
 *
 *
 * Usage
 * -----
 *
 * @code
 * typedef SomeData MyData;
 *
 * CN_CIRQ(struct MyCirq, MyData);
 *
 * void push(struct MyCirq** headp, MyData data)
 * {
 *     struct MyCirq* entry = malloc(sizeof(*entry));
 *
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
 * @brief Get the pointer to the CnBinode member of the *cirq* entry.
 *
 * @param[in] cirq The *cirq* entry pointer.
 *
 * @return A pointer to the CnBinode.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirq.
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
 * @brief Get the pointer to the data member of the *cirq* entry.
 *
 * @param[in] cirq The *cirq* entry pointer.
 *
 * @return A pointer to the data.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirq.
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
 * @brief Cast a CnBinode member out to the containing *cirq* entry.
 *
 * @param[in] ptr The pointer to the CnBinode member.
 * @param[in] type The type of the *cirq*.
 *
 * @return A pointer to the *cirq* entry.
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
 * @brief Insert, at the given position, an entry into the *cirq*.
 *
 * A call cn_cirq_ins(cirq, entry, pos) will insert the @a entry into the @a
 * cirq at the @a pos. To insert at the head use 0 for the @a pos and to insert
 * at the tail use -1 for the @a pos. The argument @a pos is optional and by
 * default it equals -1.
 *
 * @return The new head.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirq.
 */
#define cn_cirq_ins(cirq, ...) _CN_CIRQ_INS((cirq), __VA_ARGS__, -1, )

/**
 * @def cn_cirq_rem(...)
 *
 * @brief Remove, at the given position, an entry from the *cirq*.
 *
 * A call cn_cirq_rem(cirqp, pos) will remove an entry from the @a cirqp at the
 * @a pos and will return the removed entry. To remove from the head use 0 for
 * the @a pos and to remove from the tail use -1 for the @a pos. The argument
 * @a pos is optional and by default it equals 0.
 *
 * @return The removed entry.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirqp.
 */
#define cn_cirq_rem(...) _CN_CIRQ_REM(__VA_ARGS__, 0, )

/**
 * @def CN_CIRQ(name, type)
 *
 * @brief Define the *cirq*.
 *
 * @param[in] name The name of the type used for the *cirq*.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a *cirq* entry that holds the data of the type @a type.
 */
#define CN_CIRQ(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnBinode node;                                          \
		type data;                                                     \
	}

/**
 * @struct CnBinode
 *
 * @brief Doubly linked list node.
 */
struct CnBinode {
	/**
	 * @var struct CnBinode* next
	 *
	 * @brief The pointer to the next entry in the doubly linked list.
	 */
	struct CnBinode* next;

	/**
	 * @var struct CnBinode* prev
	 *
	 * @brief The pointer to the previous entry in the doubly linked list.
	 */
	struct CnBinode* prev;
};

/**
 * @fn struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos)
 *
 * @brief Gets the entry at the given position in a doubly linked list.
 *
 * @param[in] node The reference entry.
 * @param[in] pos The offset from the reference entry.
 *
 * For positive values of @a pos this will iterate over the @a next pointers and
 * for negative values of @a pos this will iterate over the @b prev pointers.
 * For 0 @a pos, this will return the reference entry.
 *
 * @return The entry at the position @a pos away from the reference entry.
 */
struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos);

/**
 * @fn struct CnBinode* cn_binode_ins(struct CnBinode* cirq, struct CnBinode* entry, int pos)
 *
 * @brief Insert, at the given position, an entry into the *cirq*.
 *
 * @param[in,out] cirq The head of the *cirq*.
 * @param[in,out] entry The new entry.
 * @param[in] pos The position at which the new entry is inserted.
 *
 * @return The new head.
 *
 * @note This function assumes that the list on which it operates is a circular list.
 */
struct CnBinode*
cn_binode_ins(struct CnBinode* cirq, struct CnBinode* entry, int pos);

/**
 * @fn struct CnBinode* cn_binode_rem(struct CnBinode** cirqp, int pos)
 *
 * @brief Remove, at the given position, an entry from the *cirq*.
 *
 * @param[in,out] cirqp The double pointer to the head of the *cirq*.
 * @param[in] pos The position of the entry that is removed.
 *
 * @return The removed entry.
 */
struct CnBinode* cn_binode_rem(struct CnBinode** cirqp, int pos);

#endif /* CN_CIRQ_H */
