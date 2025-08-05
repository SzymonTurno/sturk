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
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/rbtree.h"
#include <string.h>

static struct CnStrnode* cont(struct CnRbnode* ptr)
{
	return container_of(ptr, struct CnStrnode, node);
}

static struct CnStrnode* trycont(struct CnRbnode* ptr)
{
	return ptr ? cont(ptr) : NULL;
}

struct CnStrnode* cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node)
{
	struct CnRbnode* tmp = root ? &root->node : NULL;
	struct CnRbnode** i = &tmp;
	struct CnStrnode* p = NULL;

	ENSURE(node && node->str, ERROR, null_param);
	while (*i) {
		p = cont(*i);
		if (strcmp(node->str, p->str) < 0)
			i = &p->node.left;
		else
			i = &p->node.right;
	}
	*i = rb_link(&node->node, p ? &p->node : NULL);
	return cont(rb_insrebal(tmp, &node->node));
}

struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str)
{
	int tmp = 0;

	ENSURE(str, ERROR, null_param);
	while (root) {
		ENSURE(root->str, ERROR, null_param);
		tmp = strcmp(str, root->str);
		if (tmp < 0)
			root = trycont(root->node.left);
		else if (tmp > 0)
			root = trycont(root->node.right);
		else
			break;
	}
	return root;
}
