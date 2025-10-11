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
 * @file st/arena.h
 *
 * @brief Arena allocator.
 *
 * Solution inspired by the example from "C Interfaces and Implementations:
 * Techniques for Creating Reusable Software" by David Hanson.
 */

#ifndef ST_ARENA_H
#define ST_ARENA_H

#include <stddef.h>

struct StMemVt {
	void* (*alloc_cb)(size_t);
	void (*free_cb)(void*);
};

/**
 * @var typedef struct StArena StArena
 *
 * @brief The arena.
 */
typedef struct StArena StArena;

/**
 * @struct StArenaGroup
 *
 * @brief Collection of arena instances.
 */
struct StArenaGroup {
	/** @brief Using union @a u for explicit padding. */
	union {
		/**
		 * @var int n_elems
		 *
		 * @brief The number of elements in a collection.
		 */
		int n_elems;

		/** @var void* align */
		void* align;
	} u;

	/**
	 * @var StArena* p
	 *
	 * @brief The pointer to an arena.
	 */
	StArena* p;
};

/**
 * @fn StArena* st_arena_create(struct StArenaGroup* group, const struct StMemVt* vp)
 *
 * @brief Create an arena.
 *
 * @param[in,out] group Collection of arena instances.
 * @param[in] alloc Memory allocation callback.
 * @param[in] free Memory freeing callback.
 *
 * @return The pointer to the new arena.
 */
StArena* st_arena_create(struct StArenaGroup* group, const struct StMemVt* vp);

/**
 * @fn void st_arena_destroy(StArena* arena)
 *
 * @brief Destroy an arena.
 *
 * @param[in,out] arena The pointer to the arena.
 */
void st_arena_destroy(StArena* arena);

/**
 * @fn void* st_arena_alloc(StArena* arena, size_t size)
 *
 * @brief Allocate memory from an arena.
 *
 * @param[in,out] arena The pointer to the arena.
 * @param[in,out] size The size in bytes of the requested memory.
 *
 * @return The pointer to the allocated memory.
 */
void* st_arena_alloc(StArena* arena, size_t size);

/**
 * @fn void st_arena_free(StArena* arena)
 *
 * @brief Free all the memory allocated from a single arena.
 *
 * @param[in,out] arena The pointer to the arena.
 */
void st_arena_free(StArena* arena);

/**
 * @fn void st_arena_cleanup(struct StArenaGroup* group)
 *
 * @brief Free all the memory from an arena collection.
 *
 * @param[in,out] group The arena collection.
 */
void st_arena_cleanup(struct StArenaGroup* group);

#endif /* ST_ARENA_H */
