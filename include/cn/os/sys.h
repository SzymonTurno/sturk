/**
 * @file cn/os/sys.h
 * @brief Header file for system procedures.
 *
 * *** todo ***.
 */

#ifndef CN_SYS_H
#define CN_SYS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @fn int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...)
 * @brief *** todo ***.
 * @param[in,out] buffer Input/output.
 * @param[in] bufsz Input.
 * @param[in] format Input.
 * @param[in,out] ... Input/output.
 * @returns *** todo ***.
 */
int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...);

/**
 * @fn void cn_sysfail(void)
 * @brief *** todo ***.
 */
void cn_sysfail(void);

/**
 * @fn int cn_remove(const char* filename)
 * @brief *** todo ***.
 * @param[in] filename Input.
 * @returns *** todo ***.
 */
int cn_remove(const char* filename);

#endif /* CN_SYS_H */
