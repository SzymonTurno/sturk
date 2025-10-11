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
 * @file sturk/io/buffer.h
 *
 * @see st/io/buffer.h
 */

#ifndef STURK_IO_BUFFER_H
#define STURK_IO_BUFFER_H

#include "st/io/buffer.h"

/** @see ST_IO_EOF */
#define IO_EOF ST_IO_EOF

/** @see st_iobuffer_calclen() */
#define iobuffer_calclen st_iobuffer_calclen

/** @see st_io_init() */
#define io_init st_io_init

/** @see st_io_setp() */
#define io_setp st_io_setp

/** @see st_io_setvp() */
#define io_setvp st_io_setvp

/** @see st_io_putc() */
#define io_putc st_io_putc

/** @see st_io_getc() */
#define io_getc st_io_getc

/** @see st_io_vprint() */
#define io_vprint st_io_vprint

/** @see st_io_print() */
#define io_print st_io_print

/** @see st_io_fgets() */
#define io_fgets st_io_fgets

#endif /* ST_IO_BUFFER_H */
