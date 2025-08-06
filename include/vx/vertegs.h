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
 * @file vx/vertegs.h
 *
 * @brief Vertex.
 *
 * This header file provides data types, functions and macros that define and
 * operate on graph vertices.
 */

#ifndef VERTEGS_H
#define VERTEGS_H

#include <stddef.h>

#ifndef VX_EXCEPT

#define VX_EXCEPT(reason, file, line)

#endif /* VX_EXCEPT */

#define VX_ENSURE_MEM(v)                                                       \
	do {                                                                   \
		if ((v) == NULL) {                                             \
			VX_EXCEPT("Null param.", __FILE__, __LINE__);          \
			return NULL;                                           \
		}                                                              \
	} while (0)

struct Vertegs {
	struct Vertegs* adjyl[1];
};

static inline struct Vertegs** vx_2adjyl(struct Vertegs* v)
{
	return (struct Vertegs**)v;
}

static inline struct Vertegs* vx_4adjyl(struct Vertegs** adjyl)
{
	return (struct Vertegs*)adjyl;
}

static inline struct Vertegs* vx_upto(struct Vertegs* v, size_t edge, int pos)
{
	struct Vertegs* p = NULL;

	VX_ENSURE_MEM(v);
	while (pos-- && (p = v->adjyl[edge]))
		v = p;
	return v;
}

static inline struct Vertegs*
vx_inslist(struct Vertegs* list, struct Vertegs* entry, int pos)
{
	const size_t next = 0;
	struct Vertegs* adjyl[] = {list};
	struct Vertegs* v = vx_upto(vx_4adjyl(adjyl), next, pos);

	VX_ENSURE_MEM(entry);
	entry->adjyl[next] = v->adjyl[next];
	v->adjyl[next] = entry;
	return adjyl[next];
}

static inline struct Vertegs* vx_remlist(struct Vertegs** listp, int pos)
{
	const size_t next = 0;
	struct Vertegs* ret = NULL;
	struct Vertegs* v = vx_upto(vx_4adjyl(listp), next, pos);

	ret = v->adjyl[next];
	v->adjyl[next] = ret->adjyl[next];
	return ret;
}

static inline struct Vertegs*
vx_inscirq(struct Vertegs* cirq, struct Vertegs* entry, int pos)
{
	const size_t next = 0;
	const size_t prev = 1;
	struct Vertegs* v = NULL;

	VX_ENSURE_MEM(entry);
	if (!cirq) {
		entry->adjyl[next] = entry;
		entry->adjyl[prev] = entry;
		return entry;
	}

	if (pos > 0)
		v = vx_upto(cirq, next, pos);
	else if (pos < -1)
		v = vx_upto(cirq, prev, -(pos + 1));
	else
		v = cirq;
	entry->adjyl[next] = v;
	entry->adjyl[prev] = v->adjyl[prev];
	v->adjyl[prev] = entry;
	entry->adjyl[prev]->adjyl[next] = entry;
	return pos ? cirq : entry;
}

static inline struct Vertegs* vx_remcirq(struct Vertegs** cirqp, int pos)
{
	const size_t next = 0;
	const size_t prev = 1;
	struct Vertegs* ret = NULL;

	VX_ENSURE_MEM(cirqp);
	ret = *cirqp;
	if (pos > 0)
		ret = vx_upto(ret, next, pos);
	else if (pos < 0)
		ret = vx_upto(ret, prev, -pos);
	ret->adjyl[next]->adjyl[prev] = ret->adjyl[prev];
	ret->adjyl[prev]->adjyl[next] = ret->adjyl[next];
	if (ret == ret->adjyl[next])
		*cirqp = NULL;
	else if (ret == *cirqp)
		*cirqp = ret->adjyl[next];
	return ret;
}

#endif /* VERTEGS_H */
