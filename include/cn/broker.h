/**
 * @file cn/broker.h
 *
 * @brief Message broker.
 *
 * This header file provides data types and functions that implement the
 * publish-subscribe messaging pattern.
 *
 * The design choices:
 * 1. BST - as oppose to hash table - for implementing channels dictionary,
 * 2. memory pools for messages and queue entries,
 *
 * attempt to make this implementation suitable for memory constrained systems.
 */

#ifndef CN_BROKER_H
#define CN_BROKER_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef char CnLoad
 *
 * @brief Opaque data type that represents the message load.
 *
 * The memory for each message has two contexts:
 * 1. direct - allocated from the memory pool;
 * 2. indirect - optional, allocated within CnLoadVt::ctor.
 *
 * It is for the user to decide how to use those contexts by defining the
 * message constructor (CnLoadVt::ctor).
 *
 * Direct context is a contiguous memory block allocated from fixed-size memory
 * pool and its size is constant for all messages. The size of the
 * block is a multiple of the size of the metadata, big enough to hold one
 * instance of the metadata and one instance of the user defined load. The size
 * of the load for the direct context is defined with the CnLoadVt::size
 * callback.
 * @see CnPool
 *
 * <table>
 * <caption id="direct_context">Direct context</caption>
 * <tr><th>Array <th>Load (CnLoad*) + meta
 * <tr><td>0     <td rowspan="3">load
 * <tr><td>...
 * <tr><td>n-1
 * <tr><td>n     <td>meta
 * </table>
 *
 * The indirect context is optional and it is everything that is allocated by
 * the contructor callback - CnLoadVt::ctor and that is accessible through
 * pointers placed somewhere in the direct context.
 */
typedef char CnLoad;

/**
 * @struct CnLoadVt
 *
 * @brief Vtable for message construction.
 */
struct CnLoadVt {
	/**
	 * @var size_t (*size)(void)
	 *
	 * @brief Callback for obtaining the size of the load.
	 *
	 * Should return the size of the load in bytes.
	 *
	 * @note It is called only once - when creating the broker with
	 * cn_broker_create().
	 */
	size_t (*size)(void);

	/**
	 * @var void (*ctor)(CnLoad*, va_list)
	 *
	 * @brief Constructor callback for the message.
	 *
	 * Should allocate additional memory for the message, if needed (see
	 * "indirect context") and initialize the message - read arguments from
	 * the va_list and fill the load passed through the CnLoad pointer.
	 *
	 * @note The input va_list will hold all values passed to cn_publish()
	 * after the CnChannel argument.
	 * @see cn_publish()
	 */
	void (*ctor)(CnLoad*, va_list);

	/**
	 * @var void (*dtor)(CnLoad*)
	 *
	 * @brief Destructor callback for the message.
	 *
	 * Should free all the memory allocated by the CnLoadVt::ctor.
	 */
	void (*dtor)(CnLoad*);
};

/**
 * @var typedef struct CnBroker CnBroker
 *
 * @brief The message broker.
 *
 * CnBroker holds the list of all subscribers (CnSubscriber) in usage and
 * a dictionary of channels (CnChannel). All the messaging done through
 * channels created by the same broker will also use the same API for message
 * construction (CnLoadVt).
 */
typedef struct CnBroker CnBroker;

/**
 * @var typedef struct CnSubscriber CnSubscriber
 *
 * @brief The subscriber.
 */
typedef struct CnSubscriber CnSubscriber;

/**
 * @var typedef struct CnChannel CnChannel
 *
 * @brief The channel for messages.
 *
 * @note Channel is related to the topic through the dictionary owned by the
 * message broker.
 * @see cn_broker_search()
 */
typedef struct CnChannel CnChannel;

/**
 * @fn void cn_publish(CnChannel* ch, ...)
 *
 * @brief Broadcast the message.
 *
 * @param[in,out] ch The channel to which the message is sent.
 * @param[in] ... The list of arguments used by the CnLoadVt::ctor.
 *
 * Channels without any subscribers are allowed. Publishing to such channel is
 * safe and it does not have any meaningful behaviour.
 */
void cn_publish(CnChannel* ch, ...);

/**
 * @fn void cn_subscribe(CnSubscriber* sber, const char* topic)
 *
 * @brief Subscribe to a topic.
 *
 * @param[in,out] sber The subscriber that expresses the interest in the topic.
 * @param[in] topic The topic to which the subscription will be made.
 */
void cn_subscribe(CnSubscriber* sber, const char* topic);

/**
 * @fn CnBroker* cn_broker_create(const struct CnLoadVt* vp)
 *
 * @brief Create the message broker.
 *
 * @param[in] vp The pointer to the vtable for the CnLoad.
 *
 * The chosen vtable will influence the behaviour of the functions that are
 * responsible for constructing and receiving the messages:
 * - cn_publish(),
 * - cn_subscriber_await(),
 * - cn_subscriber_poll().
 *
 * @returns The pointer to the new broker.
 */
CnBroker* cn_broker_create(const struct CnLoadVt* vp);

/**
 * @fn void cn_broker_destroy(CnBroker* broker)
 *
 * @brief Destroy the message broker.
 *
 * @param[in] broker The pointer to the broker.
 */
void cn_broker_destroy(CnBroker* broker);

/**
 * @fn CnChannel* cn_broker_search(CnBroker* broker, const char* topic)
 *
 * @brief Find the channel that is assigned to the given topic.
 *
 * @param[in,out] broker The message broker.
 * @param[in] topic The topic. It is also used as the key for the channel's dictionary.
 *
 * This function also creates the channel if none is found.
 *
 * @returns The pointer to the channel or NULL if NULL topic was passed.
 */
CnChannel* cn_broker_search(CnBroker* broker, const char* topic);

/**
 * @fn const char* cn_channel_gettopic(const CnChannel* ch)
 *
 * @brief Get the topic for the given channel.
 *
 * @param[in] ch The channel.
 *
 * @returns The topic (named channel).
 */
const char* cn_channel_gettopic(const CnChannel* ch);

/**
 * @fn CnSubscriber* cn_subscriber_create(CnBroker* broker)
 *
 * @brief Create the subscriber.
 *
 * @param[in,out] broker The message broker.
 *
 * @returns The pointer to the new subscriber.
 */
CnSubscriber* cn_subscriber_create(CnBroker* broker);

/**
 * @fn void cn_subscriber_destroy(CnSubscriber* sber)
 *
 * @brief Destroy the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 */
void cn_subscriber_destroy(CnSubscriber* sber);

/**
 * @fn CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch)
 *
 * @brief Wait for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 * @param[in,out] ch The pointer to the channel reference.
 *
 * This function will receive the message immediately and return the load, if
 * the subscriber's message queue is not empty. Otherwise, with multithreading
 * enabled, this will block the thread that has called this function until some
 * other thread publishes to topic that the given subscriber is interested in.
 * With a single thread application, the blocking is not supported.
 *
 * If the pointer to the channel reference is not NULL, the message's source
 * channel will be returned through it.
 *
 * @returns The load.
 */
CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch);

/**
 * @fn CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch)
 *
 * @brief Poll for the messages that are wanted by the subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 * @param[in,out] ch The pointer to the channel reference.
 *
 * This function will receive the message and return the load, if the
 * subscriber's message queue is not empty. Otherwise, it will return NULL.
 *
 * If the pointer to the channel reference is not NULL, the message's source
 * channel will be returned through it.
 *
 * @returns The load.
 */
CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch);

/**
 * @fn void cn_subscriber_release(CnSubscriber* sber)
 *
 * @brief Inform the broker that the message can be released for the given subscriber.
 *
 * @param[in,out] sber The pointer to the subscriber.
 *
 * Inform the message broker that the subscriber has finished reading the
 * message. When this function is called for the last subscriber that has
 * expressed the interest in the message, the message is returned to the memory
 * pool.
 *
 * This function is also called by cn_subscriber_await() and
 * cn_subscriber_poll().
 */
void cn_subscriber_release(CnSubscriber* sber);

#endif /* CN_BROKER_H */
