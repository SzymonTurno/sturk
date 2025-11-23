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

#include "sturk/debug.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"
#include "sturk/os/sys.h"

struct StMutex {
	int locked;
	int recursive;
};

StMutex* st_mutex_create(VxBits args)
{
	StMutex* self = NEW(StMutex);

	self->locked = 0;
	self->recursive = args & MUTEX_TYPE_RECURSIVE;
	return self;
}

void st_mutex_destroy(StMutex* mutex)
{
	st_free(mutex);
}

void st_mutex_lock(StMutex* mutex)
{
	/* LCOV_EXCL_START */
	if (!mutex)
		EXCEPT(null_param);
	/* LCOV_EXCL_STOP */

	if (mutex->locked && !mutex->recursive)
		DPRINT("Fake mutex does not support context switch.");
	mutex->locked = 1;
}

int st_mutex_trylock(StMutex* mutex)
{
	/* LCOV_EXCL_START */
	if (!mutex)
		EXCEPT(null_param);
	/* LCOV_EXCL_STOP */

	if (mutex->locked && !mutex->recursive)
		return 0;
	mutex->locked = 1;
	return 1;
}

void st_mutex_unlock(StMutex* mutex)
{
	/* LCOV_EXCL_START */
	if (!mutex)
		EXCEPT(null_param);
	/* LCOV_EXCL_STOP */

	if (!mutex->locked)
		DPRINT("Unlocking an already unlocked mutex.");
	mutex->locked = 0;
}
