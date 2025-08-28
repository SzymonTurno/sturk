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
 * @file sturk/os/mutex.h
 *
 * @see st/os/mutex.h
 */

#ifndef STURK_OS_MUTEX_H
#define STURK_OS_MUTEX_H

#include "st/os/mutex.h"

/** @see ST_MUTEX_BF() */
#define MUTEX_BF ST_MUTEX_BF

/** @see ST_MUTEX_POLICY_MASK */
#define MUTEX_POLICY_MASK ST_MUTEX_POLICY_MASK

/** @see ST_MUTEX_TYPE_MASK */
#define MUTEX_TYPE_MASK ST_MUTEX_TYPE_MASK

/** @see ST_MUTEX_UNUSED_MASK */
#define MUTEX_UNUSED_MASK ST_MUTEX_UNUSED_MASK

/** @see ST_MUTEX_POLICY_NONE */
#define MUTEX_POLICY_NONE ST_MUTEX_POLICY_NONE

/** @see ST_MUTEX_POLICY_PRIO_INHERIT */
#define MUTEX_POLICY_PRIO_INHERIT ST_MUTEX_POLICY_PRIO_INHERIT

/** @see ST_MUTEX_POLICY_DELIM */
#define MUTEX_POLICY_DELIM ST_MUTEX_POLICY_DELIM

/** @see ST_MUTEX_TYPE_NONE */
#define MUTEX_TYPE_NONE ST_MUTEX_TYPE_NONE

/** @see ST_MUTEX_TYPE_RECURSIVE */
#define MUTEX_TYPE_RECURSIVE ST_MUTEX_TYPE_RECURSIVE

/** @see ST_MUTEX_TYPE_DELIM */
#define MUTEX_TYPE_DELIM ST_MUTEX_TYPE_DELIM

/** @see st_mutex_create() */
#define mutex_create st_mutex_create

/** @see st_mutex_destroy() */
#define mutex_destroy st_mutex_destroy

/** @see st_mutex_lock() */
#define mutex_lock st_mutex_lock

/** @see st_mutex_trylock() */
#define mutex_trylock st_mutex_trylock

/** @see st_mutex_unlock() */
#define mutex_unlock st_mutex_unlock

#endif /* STURK_OS_MUTEX_H */
