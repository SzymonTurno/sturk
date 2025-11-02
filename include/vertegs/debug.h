#ifndef VERTEGS_DEBUG_H
#define VERTEGS_DEBUG_H

#ifndef VX_DEBUGGING

/**
 * @def VX_DEBUGGING
 *
 * @brief Enable assertions.
 */
#define VX_DEBUGGING 0

#endif /* VX_DEBUGGING */

#if VX_DEBUGGING

#include <assert.h>
#include <stdio.h>

#define VX_ASSERT(cond) assert(cond)

#define vx_eprint(text, file, line)                                            \
	fprintf(stderr, "%s:%d: %s\n", file, line, text)

extern void vx_debug(const char* text, const char* file, int line);

#else /* not: VX_DEBUGGING */

/**
 * @def vx_eprint(text, file, line)
 *
 * @brief Log a message to stderr.
 *
 * @param[in] text The message.
 * @param[in] file The name of the source file.
 * @param[in] line The line number.
 */
#define vx_eprint(text, file, line) ((void)(text), (void)(file), (void)(line))

/**
 * @fn vx_debug(text, file, line)
 *
 * @brief Log a debug message.
 *
 * @param[in] text The message.
 * @param[in] file The name of the source file.
 * @param[in] line The line number.
 */
#define vx_debug(text, file, line)  ((void)(text), (void)(file), (void)(line))

#endif /* VX_DEBUGGING */

#ifndef VX_ASSERT

/**
 * @def VX_ASSERT(cond)
 *
 * @brief Raise an exception if the condition is not met.
 *
 * @param[in] cond The condition.
 */
#define VX_ASSERT(cond) (void)(cond)

#endif /* VX_ASSERT */

/**
 * @def VX_FAIL()
 *
 * @see VX_ASSERT()
 */
#define VX_FAIL() VX_ASSERT(!"Aborting!")

/**
 * @def VX_DEBUG(text)
 *
 * @see vx_debug()
 */
#define VX_DEBUG(text) vx_debug(text, __FILE__, __LINE__)

/**
 * @def VX_ALLOC_FAIL_REASON
 *
 * @brief Allocation failure message.
 */
#define VX_ALLOC_FAIL_REASON "Memory allocation failed."

/**
 * @def VX_ENSURE_MEM(ptr)
 *
 * @brief Raise an exception and return NULL if the pointer is NULL.
 *
 * @param[in] ptr The pointer.
 */
#define VX_ENSURE_MEM(ptr)                                                     \
	do {                                                                   \
		VX_ASSERT(ptr);                                                \
		if ((ptr) == NULL)                                             \
			return NULL;                                           \
	} while (0)

#endif /* VERTEGS_DEBUG_H */
