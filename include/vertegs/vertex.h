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
 * @file vertegs/vertex.h
 *
 * @brief Vertex.
 *
 * This header file provides data types, functions and macros that define and
 * operate on graph vertices.
 */

#ifndef VERTEGS_VERTEX_H
#define VERTEGS_VERTEX_H

#include <stddef.h>

#ifndef VX_DEBUGGING

/**
 * @def VX_DEBUGGING
 *
 * @brief Enable assertions.
 */
#define VX_DEBUGGING 0

#endif /* VX_DEBUGGING */

#if VX_DEBUGGING

#include <assert.h>
#include <stdio.h>

#define VX_ASSERT(cond) assert(cond)

extern void vx_debug(const char* text, const char* file, int line);

#else /* not: VX_DEBUGGING */

#define vx_debug(text, file, line) (void)(text)

#endif /* VX_DEBUGGING */

#define VX_DEBUG(text) vx_debug(text, __FILE__, __LINE__)

#ifndef VX_ASSERT

/**
 * @def VX_ASSERT(cond)
 *
 * @brief Raise an exception if the condition is not met.
 *
 * @param[in] cond The condition.
 */
#define VX_ASSERT(cond) (void)(cond)

#endif /* VX_ASSERT */

/**
 * @def VX_ENSURE_MEM(ptr)
 *
 * @brief Raise an exception and return NULL if the pointer is NULL.
 *
 * @param[in] ptr The pointer.
 */
#define VX_ENSURE_MEM(ptr)                                                     \
	do {                                                                   \
		VX_ASSERT(ptr);                                                \
		if ((ptr) == NULL)                                             \
			return NULL;                                           \
	} while (0)

/**
 * @struct Vertegs
 *
 * @brief The data type for a graph vertex.
 */
struct Vertegs {
	/**
	 * @var struct Vertegs* nbor[1]
	 *
	 * @brief The neighbourhood of the vertex - an array of neighbours.
	 */
	struct Vertegs* nbor[1];
};

/**
 * @fn static inline struct Vertegs* vx_4nbor(struct Vertegs** nbor)
 *
 * @brief Cast to vertex from its neighbourhood.
 *
 * @param[in] nbor The neighbourhood.
 *
 * @return The vertex.
 */
static inline struct Vertegs* vx_4nbor(struct Vertegs** nbor)
{
	return (struct Vertegs*)nbor;
}

/**
 * @fn static inline struct Vertegs* vx_walk(struct Vertegs* v, size_t edge, int len)
 *
 * @brief For a chain, in which all edges share the same index, find the other end.
 *
 * @param[in] v The first end of the chain.
 * @param[in] edge The index of all edges in the chain.
 * @param[in] len The length of the chain - the number of edges.
 *
 * @return The other end.
 */
static inline struct Vertegs* vx_walk(struct Vertegs* v, size_t edge, int len)
{
	VX_ENSURE_MEM(v);
	while (len-- && v->nbor[edge])
		v = v->nbor[edge];
	return v;
}

/**
 * @fn static inline struct Vertegs* vx_inslist(struct Vertegs* list, struct Vertegs* entry, int pos)
 *
 * @brief Insert, at a given position, an entry into a list.
 *
 * @param[in] list The head of the list.
 * @param[in] entry The new entry.
 * @param[in] pos The position.
 *
 * @return The new head.
 */
static inline struct Vertegs*
vx_inslist(struct Vertegs* list, struct Vertegs* entry, int pos)
{
	const size_t next = 0;
	struct Vertegs* nbor[] = {list};
	struct Vertegs* v = vx_walk(vx_4nbor(nbor), next, pos);

	VX_ENSURE_MEM(entry);
	entry->nbor[next] = v->nbor[next];
	v->nbor[next] = entry;
	return nbor[next];
}

/**
 * @fn static inline struct Vertegs* vx_remlist(struct Vertegs** listp, int pos)
 *
 * @brief Remove, at a given position, an entry from a list.
 *
 * @param[in] listp The pointer to the head of the list.
 * @param[in] pos The position.
 *
 * @return The removed entry.
 */
static inline struct Vertegs* vx_remlist(struct Vertegs** listp, int pos)
{
	const size_t next = 0;
	struct Vertegs* ret = NULL;
	struct Vertegs* v = vx_walk(vx_4nbor(listp), next, pos);

	ret = v->nbor[next];
	v->nbor[next] = ret->nbor[next];
	return ret;
}

/**
 * @fn static inline struct Vertegs* vx_inscirq(struct Vertegs* cirq, struct Vertegs* entry, int pos)
 *
 * @brief Insert, at a given position, an entry into a *cirq*.
 *
 * @param[in] cirq The head of the *cirq*.
 * @param[in] entry The new entry.
 * @param[in] pos The position.
 *
 * @return The new head.
 */
static inline struct Vertegs*
vx_inscirq(struct Vertegs* cirq, struct Vertegs* entry, int pos)
{
	const size_t next = 0;
	const size_t prev = 1;
	struct Vertegs* v = NULL;

	VX_ENSURE_MEM(entry);
	if (!cirq) {
		entry->nbor[next] = entry;
		entry->nbor[prev] = entry;
		return entry;
	}

	if (pos > 0)
		v = vx_walk(cirq, next, pos);
	else if (pos < -1)
		v = vx_walk(cirq, prev, -(pos + 1));
	else
		v = cirq;
	entry->nbor[next] = v;
	entry->nbor[prev] = v->nbor[prev];
	v->nbor[prev] = entry;
	entry->nbor[prev]->nbor[next] = entry;
	return pos ? cirq : entry;
}

/**
 * @fn static inline struct Vertegs* vx_remcirq(struct Vertegs** cirqp, int pos)
 *
 * @brief Remove, at a given position, an entry from a *cirq*.
 *
 * @param[in] cirqp The pointer to the head of the *cirq*.
 * @param[in] pos The position.
 *
 * @return The removed entry.
 */
static inline struct Vertegs* vx_remcirq(struct Vertegs** cirqp, int pos)
{
	const size_t next = 0;
	const size_t prev = 1;
	struct Vertegs* ret = NULL;

	VX_ENSURE_MEM(cirqp);
	ret = *cirqp;
	if (pos > 0)
		ret = vx_walk(ret, next, pos);
	else if (pos < 0)
		ret = vx_walk(ret, prev, -pos);
	ret->nbor[next]->nbor[prev] = ret->nbor[prev];
	ret->nbor[prev]->nbor[next] = ret->nbor[next];
	if (ret == ret->nbor[next])
		*cirqp = NULL;
	else if (ret == *cirqp)
		*cirqp = ret->nbor[next];
	return ret;
}

/**
 * @fn static inline struct Vertegs* vx_itnext(struct Vertegs*** it, size_t edge)
 *
 * @brief On a given edge, get the next entry from a graph iterator.
 *
 * @param[in,out] it The iterator.
 * @param[in] edge The edge index.
 *
 * @return The next entry.
 */
static inline struct Vertegs* vx_itnext(struct Vertegs*** it, size_t edge)
{
	struct Vertegs* ret = **it;

	*it = &ret->nbor[edge];
	return ret;
}

#endif /* VERTEGS_VERTEX_H */
