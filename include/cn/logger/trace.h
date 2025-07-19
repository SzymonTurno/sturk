/**
 * @file cn/logger/trace.h
 * @brief Header file for logging procedures.
 *
 * *** todo ***.
 */

#ifndef CN_LOGGER_TRACE_H
#define CN_LOGGER_TRACE_H

#include "cn/os/fstream.h"

#ifndef CN_LOGGER_EN

/**
 * @def CN_LOGGER_EN
 * @brief *** todo ***.
 */
#define CN_LOGGER_EN 0

#endif /* CN_LOGGER_EN */

/**
 * @def CN_TRACE(lvl, tag, ...)
 * @brief *** todo ***.
 * @param[in] lvl Input.
 * @param[in] tag Input.
 * @param[in] ... Input.
 */
#define CN_TRACE(lvl, tag, ...)                                                \
	do {                                                                   \
		if (CN_LOGGER_EN)                                              \
			cn_trace(lvl, tag, __VA_ARGS__);                       \
	} while (0)

/**
 * @enum CnTraceLvl
 * @brief *** todo ***.
 */
enum CnTraceLvl {
	CN_UNKNOWN = 0, /**< @brief Unknown trace level.    */
	CN_DEBUG,       /**< @brief Debug trace level.      */
	CN_INFO,        /**< @brief Info trace level.       */
	CN_WARNING,     /**< @brief Warning trace level.    */
	CN_ERROR,       /**< @brief Error trace level.      */
	CN_N_TRACE_LVLS /**< @brief Number of trace levels. */
};

/**
 * @fn void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...)
 * @brief *** todo ***.
 * @param[in] lvl Input.
 * @param[in] tag Input.
 * @param[in] format Input.
 * @param[in] ... Input.
 */
void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...);

/**
 * @fn void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream)
 * @brief *** todo ***.
 * @param[in] lvl Input.
 * @param[in,out] stream Input/output.
 */
void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream)
 * @brief *** todo ***.
 * @param[in] lvl Input.
 * @param[in,out] stream Input/output.
 */
void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_cleanup(void)
 * @brief *** todo ***.
 */
void cn_logger_cleanup(void);

#endif /* CN_LOGGER_TRACE_H */
