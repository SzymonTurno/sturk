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

#include "st/dict.h"
#include "sturk/arith.h"
#include "sturk/graph.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/rbtree.h"

static int st_strcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static struct StRbNode* rbnode_4nbor(struct Vertegs** nbor)
{
	struct StRbNode* ptr = NULL;

	ENSURE(nbor, ERROR, sanity_fail);
	ptr = graph_4vx(vx_4nbor(nbor), ptr);
	return ptr;
}

static struct StDictNode* rbnode_2dictnode(struct StRbNode* ptr)
{
	return container_of(ptr, struct StDictNode, node);
}

static struct StDictNode* dictnode_4nbor(struct Vertegs** nbor)
{
	ENSURE(nbor, ERROR, sanity_fail);
	return rbnode_2dictnode(rbnode_4nbor(nbor));
}

struct StDictNode*
st_dictnode_ins(struct StDictNode* root, struct StDictNode* node)
{
	struct Vertegs* nbor[] = {root ? graph_2vx(&root->node) : NULL};
	struct Vertegs** p = nbor;
	size_t child = 0;

	ENSURE_MEM(node, ERROR);
	ENSURE_MEM(node->str, ERROR);
	while (p[child]) {
		p = p[child]->nbor;
		if (st_strcmp(node->str, dictnode_4nbor(p)->str) < 0)
			child = BST_LEFT;
		else
			child = BST_RIGHT;
	}
	p[child] = graph_2vx(
		rb_link(&node->node, (p == nbor) ? NULL : rbnode_4nbor(p)));
	return rbnode_2dictnode(
		rb_insrebal(graph_4vx(nbor[0], &node->node), &node->node));
}

struct StDictNode* st_dictnode_find(struct StDictNode* root, const char* str)
{
	int tmp = 0;

	ENSURE_MEM(str, ERROR);
	while (root) {
		ENSURE(root->str, ERROR, null_param);
		tmp = st_strcmp(str, root->str);
		if (tmp < 0)
			root = rbnode_2dictnode(rb_left(&root->node));
		else if (tmp > 0)
			root = rbnode_2dictnode(rb_right(&root->node));
		else
			break;
	}
	return root;
}
