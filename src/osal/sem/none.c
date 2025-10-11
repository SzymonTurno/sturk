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

#include "st/os/sem.h"
#include "sturk/io/except.h"
#include "sturk/io/logger.h"
#include "sturk/os/mem.h"
#include <stddef.h>

struct StSem {
	int n;
};

StSem* st_sem_create(StBits args)
{
	StSem* self = NEW(StSem);

	(void)args;
	self->n = 0;
	return self;
}

void st_sem_destroy(StSem* sem)
{
	st_free(sem);
}

void st_sem_wait(StSem* sem)
{
	ENSURE(sem, ERROR, null_param);
	if (!sem->n)
		TRACE(WARNING, "sturk",
		      "Fake semaphore does not support context switch.");
	--sem->n;
}

bool st_sem_trywait(StSem* sem)
{
	ENSURE(sem, ERROR, null_param);
	if (sem->n) {
		--sem->n;
		return true;
	}
	return false;
}

void st_sem_post(StSem* sem)
{
	ENSURE(sem, ERROR, null_param);
	++sem->n;
}
