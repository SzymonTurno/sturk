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
 * @file sturk/rbtree.h
 *
 * @see st/rbtree.h
 */

#ifndef STURK_RBTREE_H
#define STURK_RBTREE_H

#include "st/rbtree.h"

/** @see ST_BST_INORDER */
#define BST_INORDER ST_BST_INORDER

/** @see ST_BST_PREORDER */
#define BST_PREORDER ST_BST_PREORDER

/** @see ST_BST_POSTORDER */
#define BST_POSTORDER ST_BST_POSTORDER

/** @see ST_BST_LEFT */
#define BST_LEFT ST_BST_LEFT

/** @see ST_BST_RIGHT */
#define BST_RIGHT ST_BST_RIGHT

/** @see st_rb_link() */
#define rb_link st_rb_link

/** @see st_rb_insrebal() */
#define rb_insrebal st_rb_insrebal

/** @see st_rb_left() */
#define rb_left st_rb_left

/** @see st_rb_right() */
#define rb_right st_rb_right

/** @see st_rb_parent() */
#define rb_parent st_rb_parent

/** @see st_rb_first() */
#define rb_first st_rb_first

/** @see st_rb_next() */
#define rb_next st_rb_next

#endif /* STURK_RBTREE_H */
