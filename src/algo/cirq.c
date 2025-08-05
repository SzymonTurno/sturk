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

#include "cantil/cirq.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"

struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos)
{
	while (pos > 0) {
		ENSURE(node, ERROR, null_param);
		node = node->next;
		--pos;
	}

	while (pos < 0) {
		ENSURE(node, ERROR, null_param);
		node = node->prev;
		++pos;
	}
	return node;
}

struct CnBinode*
cn_binode_ins(struct CnBinode* cirq, struct CnBinode* entry, int pos)
{
	struct CnBinode* p = NULL;

	ENSURE(entry, ERROR, null_param);
	if (cirq) {
		if (pos > 0)
			p = binode_sibl(cirq, pos);
		else if (pos < -1)
			p = binode_sibl(cirq, pos + 1);
		else
			p = cirq;
		entry->next = p;
		entry->prev = p->prev;
		p->prev = entry;
		ENSURE(entry->prev, ERROR, null_param);
		entry->prev->next = entry;
		if (!pos)
			cirq = entry;
	} else {
		entry->next = entry;
		entry->prev = entry;
		cirq = entry;
	}
	return cirq;
}

struct CnBinode* cn_binode_rem(struct CnBinode** cirqp, int pos)
{
	struct CnBinode* ret = NULL;

	ENSURE(cirqp, ERROR, null_param);
	if (*cirqp) {
		ret = binode_sibl(*cirqp, pos);
		ret->next->prev = ret->prev;
		ret->prev->next = ret->next;
		if (ret == ret->next)
			*cirqp = NULL;
		else if (ret == *cirqp)
			*cirqp = ret->next;
	}
	return ret;
}
