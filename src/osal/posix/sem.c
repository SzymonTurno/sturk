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

#include "cn/os/sem.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include <semaphore.h>

#define OK 0

struct CnSem {
	sem_t sem;
};

CnSem* cn_sem_create(CnBits args)
{
	struct CnSem* self = NEW(struct CnSem);

	(void)args;
	if (sem_init(&self->sem, 0, 0) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, sem_fail);
		cn_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}
	return self;
}

void cn_sem_destroy(CnSem* sem)
{
	if (sem_destroy(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
	cn_free(sem);
}

void cn_sem_wait(CnSem* sem)
{
	if (sem_wait(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
}

bool cn_sem_trywait(CnSem* sem)
{
	return sem_trywait(&sem->sem) == OK;
}

void cn_sem_post(CnSem* sem)
{
	if (sem_post(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
}
