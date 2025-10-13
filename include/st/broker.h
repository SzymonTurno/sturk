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
 * @file st/broker.h
 *
 * @brief Message broker.
 *
 * This header file provides data types and functions that implement the
 * publish-subscribe messaging pattern.
 */

#ifndef ST_BROKER_H
#define ST_BROKER_H

#include <stddef.h>

/**
 * @struct StMessage
 *
 * @brief Message.
 *
 * For functions that operate on the payload only the void pointer is needed.
 * The enclosing structure is used just for clarity.
 */
struct StMessage {
	/**
	 * @var void* payload
	 *
	 * @brief The payload of the message.
	 */
	void* payload;
};

/**
 * @var typedef struct StBroker StBroker
 *
 * @brief The message broker.
 *
 * The broker holds the list of all subscribers (StSubscriber) in usage and
 * a dictionary of channels (StChannel).
 */
typedef struct StBroker StBroker;

/**
 * @var typedef struct StSubscriber StSubscriber
 *
 * @brief The subscriber.
 */
typedef struct StSubscriber StSubscriber;

/**
 * @var typedef struct StChannel StChannel
 *
 * @brief The channel for messages.
 *
 * @note Channel is related to the topic through the dictionary owned by the
 * message broker.
 * @see st_broker_search()
 */
typedef struct StChannel StChannel;

/**
 * @fn void st_publish(struct StMessage* msg)
 *
 * @brief Broadcast the message and set the payload pointer to NULL.
 *
 * @param[in,out] msg The message.
 */
void st_publish(struct StMessage* msg);

/**
 * @fn void st_subscribe(StSubscriber* sber, const char* topic)
 *
 * @brief Subscribe to a topic.
 *
 * @param[in,out] sber The subscriber that expresses the interest in the topic.
 * @param[in] topic The topic to which the subscription will be made.
 */
void st_subscribe(StSubscriber* sber, const char* topic);

/**
 * @fn StBroker* st_broker_create(size_t payload_size)
 *
 * @brief Create the message broker.
 *
 * @param[in] payload_size The size of the payload for each message.
 *
 * @return The pointer to the new broker.
 */
StBroker* st_broker_create(size_t payload_size);

/**
 * @fn void st_broker_destroy(StBroker* broker)
 *
 * @brief Destroy the message broker.
 *
 * @param[in] broker The pointer to the broker.
 */
void st_broker_destroy(StBroker* broker);

/**
 * @fn StChannel* st_broker_search(StBroker* broker, const char* topic)
 *
 * @brief Find the channel that is assigned to the given topic.
 *
 * @param[in,out] broker The message broker.
 * @param[in] topic The topic. It is also used as the key for the channel's dictionary.
 *
 * This function also creates the channel if none is found.
 *
 * @return The pointer to the channel or NULL if NULL topic was passed.
 */
StChannel* st_broker_search(StBroker* broker, const char* topic);

/**
 * @fn const char* st_channel_gettopic(const StChannel* ch)
 *
 * @brief Get the topic for the given channel.
 *
 * @param[in] ch The channel.
 *
 * @return The topic (named channel).
 */
const char* st_channel_gettopic(const StChannel* ch);

/**
 * @fn struct StMessage st_channel_allocmsg(StChannel* ch)
 *
 * @brief Allocate the payload and return it enclosed in a structure.
 *
 * @param[in] ch The channel.
 *
 * @return The message structure.
 */
struct StMessage st_channel_allocmsg(StChannel* ch);

/**
 * @fn void st_message_setcb(struct StMessage msg, void (*cb)(struct StMessage))
 *
 * @brief Set a callback function for the message.
 *
 * @param[in] msg The message.
 * @param[in] cb The callback.
 *
 * The function passed will be called only once, when all of the subscribers
 * finish reading the message i.e. the subscribers counter for the message hits
 * 0.
 *
 * @note This can be useful for freeing any memory that has been allocated when
 * initializing the payload.
 */
void st_message_setcb(struct StMessage msg, void (*cb)(struct StMessage));

/**
 * @fn StChannel* st_message_getchannel(struct StMessage msg)
 *
 * @brief Get the source channel of the message.
 *
 * @param[in] msg The message.
 *
 * @return The channel.
 */
StChannel* st_message_getchannel(struct StMessage msg);

/**
 * @fn StSubscriber* st_subscriber_create(StBroker* broker)
 *
 * @brief Create the subscriber.
 *
 * @param[in,out] broker The message broker.
 *
 * @return The pointer to the new subscriber.
 */
StSubscriber* st_subscriber_create(StBroker* broker);

/**
 * @fn void st_subscriber_destroy(StSubscriber* sber)
 *
 * @brief Destroy the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 */
void st_subscriber_destroy(StSubscriber* sber);

/**
 * @fn StMessage* st_subscriber_await(StSubscriber* sber)
 *
 * @brief Wait for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * This function will return the payload of a message immediately, if there are
 * any messages in the subscriber's queue. If the subscriber's queue is empty,
 * with multithreading enabled, this will block the thread that has called this
 * function until some other thread publishes to topic that the given
 * subscriber is interested in. With a single thread application, the blocking
 * is not supported.
 *
 * @return The payload.
 */
struct StMessage st_subscriber_await(StSubscriber* sber);

/**
 * @fn void* st_subscriber_poll(StSubscriber* sber)
 *
 * @brief Poll for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * This function will return the payload of a message, if there are any messages
 * in the subscriber's queue. If the subscriber's queue is empty, it will return
 * NULL.
 *
 * @return The payload.
 */
struct StMessage st_subscriber_poll(StSubscriber* sber);

/**
 * @fn void st_subscriber_unload(StSubscriber* sber)
 *
 * @brief Inform the broker that the message can be released for the given subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * Release the last message received by the subscriber - decrement the
 * subscribers counter for the message. When the counter hits 0, the message is
 * returned to the memory pool.
 *
 * @note Functions st_subscriber_await() and st_subscriber_poll() call
 * st_subscriber_unload() automatically, there is no need to release the last
 * received message if the subscriber attempts to receive another message.
 */
void st_subscriber_unload(StSubscriber* sber);

#endif /* ST_BROKER_H */
