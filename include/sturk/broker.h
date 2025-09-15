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
 * @file sturk/broker.h
 *
 * @see st/broker.h
 */

#ifndef STURK_BROKER_H
#define STURK_BROKER_H

#include "st/broker.h"

/** @see st_publish() */
#define publish st_publish

/** @see st_subscribe() */
#define subscribe st_subscribe

/** @see st_broker_create() */
#define broker_create st_broker_create

/** @see st_broker_destroy() */
#define broker_destroy st_broker_destroy

/** @see st_broker_search() */
#define broker_search st_broker_search

/** @see st_channel_gettopic() */
#define channel_gettopic st_channel_gettopic

/** @see st_subscriber_create() */
#define subscriber_create st_subscriber_create

/** @see st_subscriber_destroy() */
#define subscriber_destroy st_subscriber_destroy

/** @see st_subscriber_await() */
#define subscriber_await st_subscriber_await

/** @see st_subscriber_poll() */
#define subscriber_poll st_subscriber_poll

/** @see st_subscriber_unload() */
#define subscriber_unload st_subscriber_unload

#endif /* STURK_BROKER_H */
