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
 * @file sturk/str.h
 *
 * @see cn/str.h
 */

#ifndef STURK_STR_H
#define STURK_STR_H

#include "cn/str.h"

/** @see cn_newstr() */
#define newstr cn_newstr

/** @see cn_strlist_ins() */
#define strlist_ins cn_strlist_ins

/** @see cn_strlist_rem() */
#define strlist_rem cn_strlist_rem

/** @see cn_strq_ins() */
#define strq_ins cn_strq_ins

/** @see cn_strq_rem() */
#define strq_rem cn_strq_rem

/** @see cn_strbag_ins() */
#define strbag_ins cn_strbag_ins

/** @see cn_strbag_rem() */
#define strbag_rem cn_strbag_rem

/** @see cn_strbag_count() */
#define strbag_count cn_strbag_count

/** @see cn_strbag_destroy() */
#define strbag_destroy cn_strbag_destroy

#endif /* STURK_STR_H */
