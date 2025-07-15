/**
 * \file cn/logger/sink.h
 * \brief Header file with helper data structure for logger utility.
 *
 * *** todo ***.
 */

#ifndef CN_LOGGER_SINK_H
#define CN_LOGGER_SINK_H

#include "cn/os/fstream.h"

/**
 * \var typedef struct CnLogsink CnLogsink
 * \brief *** todo ***.
 */
typedef struct CnLogsink CnLogsink;

/**
 * \fn CnLogsink* cn_logsink_create(void)
 * \brief *** todo ***.
 * \returns *** todo ***.
 */
CnLogsink* cn_logsink_create(void);

/**
 * \fn void cn_logsink_destroy(CnLogsink* sink)
 * \brief *** todo ***.
 * \param[in,out] sink Input/output.
 */
void cn_logsink_destroy(CnLogsink* sink);

/**
 * \fn void cn_logsink_ins(CnLogsink* sink, CnFstream* stream)
 * \brief *** todo ***.
 * \param[in,out] sink Input/output.
 * \param[in,out] stream Input/output.
 */
void cn_logsink_ins(CnLogsink* sink, CnFstream* stream);

/**
 * \fn void cn_logsink_rem(CnLogsink* sink, CnFstream* stream)
 * \brief *** todo ***.
 * \param[in,out] sink Input/output.
 * \param[in,out] stream Input/output.
 */
void cn_logsink_rem(CnLogsink* sink, CnFstream* stream);

/**
 * \fn void cn_logsink_vprint(CnLogsink* sink, const char* format, va_list vlist)
 * \brief *** todo ***.
 * \param[in,out] sink Input/output.
 * \param[in] format Input.
 * \param[in] vlist Input.
 */
void cn_logsink_vprint(CnLogsink* sink, const char* format, va_list vlist);

#endif /* CN_LOGGER_SINK_H */
