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

#include "sturk/dict.h"
#include "sturk/arith.h"
#include "sturk/graph.h"
#include "sturk/io/except.h"
#include "sturk/io/logger.h"
#include "sturk/rbtree.h"
#include "sturk/str.h"

int st_strcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

struct StDictNode*
st_dictnode_ins(struct StDictNode* root, struct StDictNode* node)
{
	struct StDictNode** i = NULL;
	struct StDictNode* p = NULL;

	ENSURE_MEM(node, ERROR);
	ENSURE_MEM(*graph_datap(node), ERROR);
	i = &root;
	while (*i) {
		p = *i;
		if (st_strcmp(*graph_datap(node), *graph_datap(p)) < 0)
			graphit_next(&i, VX_RB_LEFT);
		else
			graphit_next(&i, VX_RB_RIGHT);
	}
	*i = rb_link(node, p);
	return rb_insrebal(root, node);
}

struct StDictNode* st_dictnode_find(struct StDictNode* root, const char* str)
{
	struct StDictNode** i = NULL;
	int tmp = 0;

	ENSURE_MEM(str, ERROR);
	i = &root;
	while (*i) {
		ENSURE(*graph_datap(*i), ERROR, null_param);
		tmp = st_strcmp(str, *graph_datap(*i));
		if (tmp < 0)
			graphit_next(&i, VX_RB_LEFT);
		else if (tmp > 0)
			graphit_next(&i, VX_RB_RIGHT);
		else
			break;
	}
	return *i;
}
