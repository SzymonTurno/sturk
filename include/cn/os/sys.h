/**
 * @file cn/os/sys.h
 *
 * @brief System procedures.
 */

#ifndef CN_SYS_H
#define CN_SYS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @fn int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...)
 *
 * @see snprintf()
 */
int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...);

/**
 * @fn void cn_sysfail(void)
 *
 * @brief Call exit(EXIT_FAILURE).
 */
void cn_sysfail(void);

/**
 * @fn int cn_remove(const char* filename)
 *
 * @see remove()
 */
int cn_remove(const char* filename);

#endif /* CN_SYS_H */
