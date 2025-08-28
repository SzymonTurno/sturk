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
 * @file sturk/logger/trace.h
 *
 * @see st/logger/trace.h
 */

#ifndef STURK_LOGGER_TRACE_H
#define STURK_LOGGER_TRACE_H

#include "st/logger/trace.h"

/** @see ST_LOGGER_EN */
#define LOGGER_EN ST_LOGGER_EN

/** @see ST_TRACE() */
#define TRACE ST_TRACE

/** @see ST_UNKNOWN */
#define UNKNOWN ST_UNKNOWN

/** @see ST_DEBUG */
#define DEBUG ST_DEBUG

/** @see ST_INFO */
#define INFO ST_INFO

/** @see ST_WARNING */
#define WARNING ST_WARNING

/** @see ST_ERROR */
#define ERROR ST_ERROR

/** @see ST_N_TRACE_LVLS */
#define N_TRACE_LVLS ST_N_TRACE_LVLS

/** @see st_trace() */
#define trace st_trace

/** @see st_logger_attach() */
#define logger_attach st_logger_attach

/** @see st_logger_detach() */
#define logger_detach st_logger_detach

/** @see st_logger_cleanup() */
#define logger_cleanup st_logger_cleanup

#endif /* STURK_LOGGER_TRACE_H */
