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

#include "cantil/list.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"

struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos)
{
	ENSURE(nodep, ERROR, null_param);
	for (; *nodep && pos--; nodep = &(*nodep)->next)
		;
	return nodep;
}

struct CnUnnode*
cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node, int pos)
{
	struct CnUnnode** i = list_hand(&head, pos);

	ENSURE(node, ERROR, null_param);
	node->next = *i;
	*i = node;
	return head;
}

struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos)
{
	struct CnUnnode** i = list_hand(headp, pos);
	struct CnUnnode* ret = *i;

	ENSURE(*i, ERROR, null_param);
	*i = (*i)->next;
	return ret;
}
