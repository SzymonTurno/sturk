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
 * @see st/dict.h
 */

#ifndef STURK_DICT_H
#define STURK_DICT_H

#include "st/dict.h"

/** @see st_dict_cast() */
#define dict_cast st_dict_cast

/** @see st_dict_setk() */
#define dict_setk st_dict_setk

/** @see st_dict_getk() */
#define dict_getk st_dict_getk

/** @see st_dict_datap() */
#define dict_datap st_dict_datap

/** @see st_dict_ins() */
#define dict_ins st_dict_ins

/** @see st_dict_find() */
#define dict_find st_dict_find

/** @see st_dict_first() */
#define dict_first st_dict_first

/** @see st_dict_next() */
#define dict_next st_dict_next

/** @see ST_DICT() */
#define DICT ST_DICT

/** @see st_dictnode_ins() */
#define dictnode_ins st_dictnode_ins

/** @see st_dictnode_find() */
#define dictnode_find st_dictnode_find

/** @see st_dictnode_from() */
#define dictnode_from st_dictnode_from

#endif /* STURK_DICT_H */
