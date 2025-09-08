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

#include "st/os/fstream.h"
#include "osal/unistd.h"
#include <stdio.h>

StFstream* st_fopen(const char* filename, const char* mode)
{
	return (StFstream*)fopen(filename, mode);
}

void st_fclose(StFstream* stream)
{
	fclose((FILE*)stream);
}

char* st_fgets(char* str, int size, StFstream* stream)
{
	return fgets(str, size, (FILE*)stream);
}

int st_fseekset(StFstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_SET);
}

StFstream* st_stdout(void)
{
	return (StFstream*)stdout;
}

StFstream* st_stderr(void)
{
	return (StFstream*)stderr;
}

int st_vfprintf(StFstream* stream, const char* format, va_list vlist)
{
	return vfprintf((FILE*)stream, format, vlist);
}
