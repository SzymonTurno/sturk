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
 * @file sturk/dict.h
 *
 * @see cn/dict.h
 */

#ifndef STURK_DICT_H
#define STURK_DICT_H

#include "cn/dict.h"

/** @see cn_dict_cast() */
#define dict_cast cn_dict_cast

/** @see cn_dict_setk() */
#define dict_setk cn_dict_setk

/** @see cn_dict_getk() */
#define dict_getk cn_dict_getk

/** @see cn_dict_datap() */
#define dict_datap cn_dict_datap

/** @see cn_dict_ins() */
#define dict_ins cn_dict_ins

/** @see cn_dict_find() */
#define dict_find cn_dict_find

/** @see cn_dict_first() */
#define dict_first cn_dict_first

/** @see cn_dict_next() */
#define dict_next cn_dict_next

/** @see CN_DICT() */
#define DICT CN_DICT

/** @see cn_dictnode_ins() */
#define dictnode_ins cn_dictnode_ins

/** @see cn_dictnode_find() */
#define dictnode_find cn_dictnode_find

/** @see cn_dictnode_from() */
#define dictnode_from cn_dictnode_from

#endif /* STURK_DICT_H */
