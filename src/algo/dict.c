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

#include "cn/dict.h"
#include "cantil/arith.h"
#include "cantil/graph.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/rbtree.h"
#include <string.h>

static struct CnRbnode* rbnode_4adjyl(struct Vertegs** adjyl)
{
	struct CnRbnode* ptr = NULL;

	ENSURE(adjyl, ERROR, sanity_fail);
	ptr = graph_4vx(vx_4adjyl(adjyl), ptr);
	return ptr;
}

static struct CnStrnode* rbnode_2strnode(struct CnRbnode* ptr)
{
	return container_of(ptr, struct CnStrnode, node);
}

static struct CnStrnode* strnode_4adjyl(struct Vertegs** adjyl)
{
	ENSURE(adjyl, ERROR, sanity_fail);
	return rbnode_2strnode(rbnode_4adjyl(adjyl));
}

struct CnStrnode* cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node)
{
	struct Vertegs* adjyl[] = {root ? graph_2vx(&root->node) : NULL};
	struct Vertegs** p = adjyl;
	size_t child = 0;

	ENSURE_MEM(node, ERROR);
	ENSURE_MEM(node->str, ERROR);
	while (p[child]) {
		p = vx_2adjyl(p[child]);
		if (strcmp(node->str, strnode_4adjyl(p)->str) < 0)
			child = RB_LEFT;
		else
			child = RB_RIGHT;
	}
	p[child] = graph_2vx(
		rb_link(&node->node, (p == adjyl) ? NULL : rbnode_4adjyl(p)));
	return rbnode_2strnode(
		rb_insrebal(graph_4vx(adjyl[0], &node->node), &node->node));
}

struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str)
{
	int tmp = 0;

	ENSURE_MEM(str, ERROR);
	while (root) {
		ENSURE(root->str, ERROR, null_param);
		tmp = strcmp(str, root->str);
		if (tmp < 0)
			root = rbnode_2strnode(rb_left(&root->node));
		else if (tmp > 0)
			root = rbnode_2strnode(rb_right(&root->node));
		else
			break;
	}
	return root;
}
