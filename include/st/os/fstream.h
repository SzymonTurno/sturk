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
 * @file st/os/fstream.h
 *
 * @brief File stream.
 */

#ifndef ST_FSTREAM_H
#define ST_FSTREAM_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef struct StFstream StFstream
 *
 * @brief File stream.
 *
 * @see FILE
 */
typedef struct StFstream StFstream;

/**
 * @fn StFstream* st_fopen(const char* filename, const char* mode)
 *
 * @see fopen()
 */
StFstream* st_fopen(const char* filename, const char* mode);

/**
 * @fn void st_fclose(StFstream* stream)
 *
 * @see fclose()
 */
void st_fclose(StFstream* stream);

/**
 * @fn char* st_fgets(char* str, int size, StFstream* stream)
 *
 * @see fgets()
 */
char* st_fgets(char* str, int size, StFstream* stream);

/**
 * @fn int st_fseekset(StFstream* stream, long int offset)
 *
 * This is fseek() with mode set to SEEK_SET.
 *
 * @see fseek()
 */
int st_fseekset(StFstream* stream, long int offset);

/**
 * @fn StFstream* st_stdout(void)
 *
 * @return The stdout stream.
 *
 * @see stdout
 */
StFstream* st_stdout(void);

/**
 * @fn StFstream* st_stderr(void)
 *
 * @return The stderr stream.
 *
 * @see stderr
 */
StFstream* st_stderr(void);

/**
 * @fn int st_vfprintf(StFstream* stream, const char* format, va_list vlist)
 *
 * @see vfprintf()
 */
int st_vfprintf(StFstream* stream, const char* format, va_list vlist);

#endif /* ST_FSTREAM_H */
