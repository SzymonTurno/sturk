/**
 * @file cn/logger/streambag.h
 *
 * @brief Bag of streams.
 */

#ifndef CN_LOGGER_STREAMBAG_H
#define CN_LOGGER_STREAMBAG_H

#include "cn/os/fstream.h"

/**
 * @var typedef struct CnStreambag CnStreambag
 *
 * @brief A bag of streams.
 */
typedef struct CnStreambag CnStreambag;

/**
 * @fn CnStreambag* cn_streambag_create(void)
 *
 * @brief Create a new empty bag of streams.
 *
 * @return An empty bag of streams.
 */
CnStreambag* cn_streambag_create(void);

/**
 * @fn void cn_streambag_destroy(CnStreambag* bag)
 *
 * @brief Destroy a bag of streams
 *
 * @param[in,out] bag The bag.
 */
void cn_streambag_destroy(CnStreambag* bag);

/**
 * @fn void cn_streambag_ins(CnStreambag* bag, CnFstream* stream)
 *
 * @brief Insert a stream into a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] stream The stream.
 */
void cn_streambag_ins(CnStreambag* bag, CnFstream* stream);

/**
 * @fn void cn_streambag_rem(CnStreambag* bag, CnFstream* stream)
 *
 * @brief Remove a stream from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] stream The stream.
 */
void cn_streambag_rem(CnStreambag* bag, CnFstream* stream);

/**
 * @fn void cn_streambag_vprint(CnStreambag* bag, const char* format, va_list vlist)
 *
 * @brief Print to all streams from a bag.
 *
 * @param[in,out] bag The bag.
 * @param[in] format The format string.
 * @param[in] vlist The list of arguments.
 */
void cn_streambag_vprint(CnStreambag* bag, const char* format, va_list vlist);

#endif /* CN_LOGGER_STREAMBAG_H */
