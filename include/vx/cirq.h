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
 * @file vx/cirq.h
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
 * VX_CIRQ(struct MyCirq, MyData);
 *
 * void push(struct MyCirq** headp, MyData data)
 * {
 *     struct MyCirq* entry = malloc(sizeof(*entry));
 *
 *     *vx_graph_data(entry) = data;
 *     *headp = vx_cirq_ins(*headp, entry);
 * }
 * @endcode
 */

#ifndef VX_CIRQ_H
#define VX_CIRQ_H

#include "vx/graph.h"

/* @cond */
#define VX__CIRQ_INS(cirq, entry, pos, ...)                                    \
	(vx_graph_4vx(                                                         \
		vx_inscirq(vx_graph_2vx(cirq), vx_graph_2vx(entry), (pos)),    \
		(entry)))

#define VX__CIRQ_REM(cirqp, pos, ...)                                          \
	(vx_graph_4vx(vx_remcirq(vx_graph_2adjyl(cirqp), (pos)), *(cirqp)))
/* @endcond */

/**
 * @def VX_CIRQ(name, type)
 *
 * @brief Define the *cirq*.
 *
 * @param[in] name The name of the type used for the *cirq*.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a *cirq* entry that holds the data of the type @a type.
 */
#define VX_CIRQ(name, type) VX_GRAPH(name, 2, type)

/**
 * @def vx_cirq_ins(cirq, ...)
 *
 * @brief Insert, at the given position, an entry into the *cirq*.
 *
 * A call vx_cirq_ins(cirq, entry, pos) will insert the @a entry into the @a
 * cirq at the @a pos. To insert at the head use 0 for the @a pos and to insert
 * at the tail use -1 for the @a pos. The argument @a pos is optional and by
 * default it equals -1.
 *
 * @return The new head.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirq.
 */
#define vx_cirq_ins(cirq, ...) VX__CIRQ_INS((cirq), __VA_ARGS__, -1, )

/**
 * @def vx_cirq_rem(...)
 *
 * @brief Remove, at the given position, an entry from the *cirq*.
 *
 * A call vx_cirq_rem(cirqp, pos) will remove an entry from the @a cirqp at the
 * @a pos and will return the removed entry. To remove from the head use 0 for
 * the @a pos and to remove from the tail use -1 for the @a pos. The argument
 * @a pos is optional and by default it equals 0.
 *
 * @return The removed entry.
 *
 * @note Compile with the GNU extension to enable a type check for the @a cirqp.
 */
#define vx_cirq_rem(...) VX__CIRQ_REM(__VA_ARGS__, 0, )

#endif /* VX_CIRQ_H */
