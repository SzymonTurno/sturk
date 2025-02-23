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
 * @file cn/os/fstream.h
 *
 * @brief File stream.
 */

#ifndef CN_FSTREAM_H
#define CN_FSTREAM_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef struct CnFstream CnFstream
 *
 * @brief File stream.
 *
 * @see FILE
 */
typedef struct CnFstream CnFstream;

/**
 * @fn CnFstream* cn_fopen(const char* filename, const char* mode)
 *
 * @see fopen()
 */
CnFstream* cn_fopen(const char* filename, const char* mode);

/**
 * @fn void cn_fclose(CnFstream* stream)
 *
 * @see fclose()
 */
void cn_fclose(CnFstream* stream);

/**
 * @fn char* cn_fgets(char* str, int size, CnFstream* stream)
 *
 * @see fgets()
 */
char* cn_fgets(char* str, int size, CnFstream* stream);

/**
 * @fn int cn_fseekset(CnFstream* stream, long int offset)
 *
 * This is fseek() with mode set to SEEK_SET.
 *
 * @see fseek()
 */
int cn_fseekset(CnFstream* stream, long int offset);

/**
 * @fn CnFstream* cn_stdout(void)
 *
 * @return The stdout stream.
 *
 * @see stdout
 */
CnFstream* cn_stdout(void);

/**
 * @fn CnFstream* cn_stderr(void)
 *
 * @return The stderr stream.
 *
 * @see stderr
 */
CnFstream* cn_stderr(void);

/**
 * @fn int cn_vfprintf(CnFstream* stream, const char* format, va_list vlist)
 *
 * @see vfprintf()
 */
int cn_vfprintf(CnFstream* stream, const char* format, va_list vlist);

#endif /* CN_FSTREAM_H */
