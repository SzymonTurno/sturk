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

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef char StLoad
 *
 * @brief Opaque data type that represents the message load.
 *
 * The memory for each message has two contexts:
 * 1. static - allocated from the memory pool;
 * 2. dynamic - optional, allocated within StLoadVt::ctor.
 *
 * It is for the user to decide how to use those contexts by defining the
 * message constructor (StLoadVt::ctor).
 *
 * Static context is a contiguous memory block allocated from a fixed-size
 * memory pool and its size is constant for all messages. The size of the
 * block is a multiple of the size of the metadata, big enough to hold one
 * instance of the metadata and one instance of the user defined load. The size
 * of the load for the static context is defined with the StLoadVt::size
 * callback.
 *
 * @see StPool
 *
 * Table: Static context
 *
 * | Array | meta + load (StLoad*) |
 * | ----- | --------------------- |
 * | 0     | meta                  |
 * | ...   | load                  |
 * | n - 1 | ^                     |
 * | n     | ^                     |
 *
 * The dynamic context is optional and it is everything that is allocated by
 * the contructor callback - StLoadVt::ctor and that is accessible through
 * pointers placed somewhere in the load.
 */
typedef char StLoad;

/**
 * @struct StLoadVt
 *
 * @brief Vtable for message construction.
 */
struct StLoadVt {
	/**
	 * @var size_t (*size)(void)
	 *
	 * @brief Callback for obtaining the size of the load.
	 *
	 * Should return the size of the load in bytes.
	 */
	size_t (*size)(void);

	/**
	 * @var void (*ctor)(StLoad*, va_list)
	 *
	 * @brief Constructor callback for the message.
	 *
	 * Should allocate additional memory for the message, if needed (see
	 * "dynamic context") and initialize the message - read arguments from
	 * the va_list and fill the load passed through the StLoad pointer.
	 *
	 * @note The input va_list will hold all values passed to st_publish()
	 * after the StChannel argument.
	 * @see st_publish()
	 */
	void (*ctor)(StLoad*, va_list);

	/**
	 * @var void (*dtor)(StLoad*)
	 *
	 * @brief Destructor callback for the message.
	 *
	 * Should free all the memory allocated by the StLoadVt::ctor.
	 */
	void (*dtor)(StLoad*);
};

/**
 * @var typedef struct StBroker StBroker
 *
 * @brief The message broker.
 *
 * The broker holds the list of all subscribers (StSubscriber) in usage and
 * a dictionary of channels (StChannel). All the messaging done through
 * channels created by the same broker will also use the same API for message
 * construction (StLoadVt).
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
 * @fn void st_publish(StChannel* ch, ...)
 *
 * @brief Broadcast the message.
 *
 * @param[in,out] ch The channel to which the message is sent.
 * @param[in] ... The list of arguments passed to the StLoadVt::ctor.
 *
 * @note Channels without any subscribers are allowed. Publishing to such
 * channel is safe and it does not have any meaningful behaviour (it does
 * "nothing").
 */
void st_publish(StChannel* ch, ...);

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
 * @fn StBroker* st_broker_create(const struct StLoadVt* vp)
 *
 * @brief Create the message broker.
 *
 * @param[in] vp The pointer to the vtable for the StLoad.
 *
 * The chosen vtable will influence the behaviour of the functions that are
 * responsible for constructing and receiving the messages:
 * - st_publish(),
 * - st_subscriber_await(),
 * - st_subscriber_poll().
 *
 * @return The pointer to the new broker.
 */
StBroker* st_broker_create(const struct StLoadVt* vp);

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
 * @fn StLoad* st_subscriber_await(StSubscriber* sber)
 *
 * @brief Wait for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * This function will receive the message immediately and return the load, if
 * the subscriber's message queue is not empty. Otherwise, with multithreading
 * enabled, this will block the thread that has called this function until some
 * other thread publishes to topic that the given subscriber is interested in.
 * With a single thread application, the blocking is not supported.
 *
 * @return The load.
 */
StLoad* st_subscriber_await(StSubscriber* sber);

/**
 * @fn StLoad* st_subscriber_poll(StSubscriber* sber)
 *
 * @brief Poll for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * This function will receive the message and return the load, if the
 * subscriber's message queue is not empty. Otherwise, it will return NULL.
 *
 * @return The load.
 */
StLoad* st_subscriber_poll(StSubscriber* sber);

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

/**
 * @fn StChannel* st_load_getchan(const StLoad* load)
 *
 * @brief Get the source channel of the message.
 *
 * @param[in] load The pointer to the message load.
 *
 * @return The channel
 */
StChannel* st_load_getchan(const StLoad* load);

#endif /* ST_BROKER_H */
