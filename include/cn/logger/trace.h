/**
 * @file cn/logger/trace.h
 *
 * @brief Trace.
 */

#ifndef CN_LOGGER_TRACE_H
#define CN_LOGGER_TRACE_H

#include "cn/os/fstream.h"

#ifndef CN_LOGGER_EN

/**
 * @def CN_LOGGER_EN
 *
 * @brief Logger enabled.
 */
#define CN_LOGGER_EN 0

#endif /* CN_LOGGER_EN */

/**
 * @def CN_TRACE(lvl, tag, ...)
 *
 * @brief Log to the streams that are attached to the logger.
 *
 * This is almost the same as cn_trace(). The main difference is that it will
 * not compile if the CN_LOGGER_EN is 0.
 *
 * @see cn_trace()
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
	CN_UNKNOWN = 0, /**< unknown trace level.    */
	CN_DEBUG,       /**< debug trace level.      */
	CN_INFO,        /**< info trace level.       */
	CN_WARNING,     /**< warning trace level.    */
	CN_ERROR,       /**< error trace level.      */
	CN_N_TRACE_LVLS /**< number of trace levels. */
};

/**
 * @fn void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...)
 *
 * @brief Log to the streams that are attached to the logger.
 *
 * @param[in] lvl The log level.
 * @param[in] tag The tag string.
 * @param[in] format The format string.
 * @param[in] ... The list of arguments.
 */
void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...);

/**
 * @fn void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream)
 *
 * @brief Attach a stream to the logger.
 *
 * @param[in] lvl The log level.
 * @param[in,out] stream The stream.
 */
void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream)
 *
 * @brief Detach a stream from the logger.
 *
 * @param[in] lvl The log level.
 * @param[in,out] stream The stream.
 */
void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream);

/**
 * @fn void cn_logger_cleanup(void)
 *
 * @brief Detach all streams and free all the memory allocated by the logger.
 */
void cn_logger_cleanup(void);

#endif /* CN_LOGGER_TRACE_H */
