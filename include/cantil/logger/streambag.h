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
 * @file cantil/logger/streambag.h
 *
 * @see cn/logger/streambag.h
 */

#ifndef CANTIL_LOGGER_STREAMBAG_H
#define CANTIL_LOGGER_STREAMBAG_H

#include "cn/logger/streambag.h"

/** @see cn_streambag_create() */
#define streambag_create cn_streambag_create

/** @see cn_streambag_destroy() */
#define streambag_destroy cn_streambag_destroy

/** @see cn_streambag_ins() */
#define streambag_ins cn_streambag_ins

/** @see cn_streambag_rem() */
#define streambag_rem cn_streambag_rem

/** @see cn_streambag_vprint() */
#define streambag_vprint cn_streambag_vprint

/** @see cn_streambag_count() */
#define streambag_count cn_streambag_count

#endif /* CANTIL_LOGGER_STREAMBAG_H */
