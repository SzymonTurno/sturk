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
 * @file cantil/rbtree.h
 *
 * @see cn/rbtree.h
 */

#ifndef CANTIL_RBTREE_H
#define CANTIL_RBTREE_H

#include "cn/rbtree.h"

/** @see CN_BST_INORDER */
#define BST_INORDER CN_BST_INORDER

/** @see CN_BST_PREORDER */
#define BST_PREORDER CN_BST_PREORDER

/** @see CN_BST_POSTORDER */
#define BST_POSTORDER CN_BST_POSTORDER

/** @see CN_RB_LEFT */
#define RB_LEFT CN_RB_LEFT

/** @see CN_RB_RIGHT */
#define RB_RIGHT CN_RB_RIGHT

/** @see cn_rb_link() */
#define rb_link cn_rb_link

/** @see cn_rb_insrebal() */
#define rb_insrebal cn_rb_insrebal

/** @see cn_rb_left() */
#define rb_left cn_rb_left

/** @see cn_rb_right() */
#define rb_right cn_rb_right

/** @see cn_rb_parent() */
#define rb_parent cn_rb_parent

/** @see cn_rb_first() */
#define rb_first cn_rb_first

/** @see cn_rb_next() */
#define rb_next cn_rb_next

#endif /* CANTIL_RBTREE_H */
