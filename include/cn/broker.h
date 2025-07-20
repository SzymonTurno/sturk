/**
 * @file cn/broker.h
 *
 * @brief Message broker.
 *
 * This file defines data types and functions that implement the
 * publish-subscribe messaging pattern. The design choices:
 * 1. BST - as oppose to hash table - for implementing channels dictionary,
 * 2. memory pools for messages and queue entries,
 * attempt to make this implementation suitable for memory constrained systems.
 */

#ifndef CN_BROKER_H
#define CN_BROKER_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef char CnLoad
 *
 * @brief Opaque data type that represents the load.
 *
 * The memory for each message has two contexts:
 * 1. direct - allocated from the memory pool;
 * 2. indirect - optional, allocated within CnLoadVt::ctor.
 *
 * It is for the user to decide how to use those contexts by defining the
 * message constructor (@see CnLoadVt::ctor).
 *
 * Direct context is a contiguous memory block allocated from fixed-size memory
 * pool (@see CnPool) and its size is constant for all messages. The size of the
 * block is a multiple of the size of the metadata, big enough to hold one
 * instance of the metadata and one instance of the user defined load. The size
 * of the load for the direct context is defined with CnLoadVt::size callback.
 *
 * DIRECT CONTEXT
 * ----------+------+ <-- The load pointer (CnLoad*) always points here.
 *  msg[0]   |      |     @see CnLoadVt::ctor
 * ----------|      |     @see CnLoadVt::dtor
 *  ...      | load |     @see cn_subscriber_await()
 * ----------|      |     @see cn_subscriber_poll()
 *  msg[n-1] |      |
 * ----------+------+
 *  msg[n]   | meta |
 * ----------+------+
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
	 * va_list and fill the load passed through CnLoad pointer.
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
	 * Should free all the memory allocated by CnLoadVt::ctor.
	 */
	void (*dtor)(CnLoad*);
};

/**
 * @var typedef struct CnBroker CnBroker
 *
 * @brief The message broker.
 *
 * CnBroker holds the list of all subscribers (@see CnSubscriber) in usage and
 * a dictionary of channels (@see CnChannels). All the messaging done through
 * channels created by the same broker will also use the same API for message
 * construction (@see CnLoadVt).
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
 * message broker (@see cn_broker_search()).
 */
typedef struct CnChannel CnChannel;

/**
 * @fn void cn_publish(CnChannel* ch, ...)
 *
 * @brief Broadcast the message.
 *
 * @param[in,out] ch The channel to which the message is sent.
 * @param[in] ... list of arguments used by CnLoadVt::ctor.
 */
void cn_publish(CnChannel* ch, ...);

/**
 * @fn void cn_subscribe(CnSubscriber* sber, const char* topic)
 *
 * @brief Subscribe to topic.
 *.
 * @param[in,out] sber The subscriber that expresses the interest in the topic.
 * @param[in] topic The topic to which subscription will be made.
 */
void cn_subscribe(CnSubscriber* sber, const char* topic);

/**
 * @fn CnBroker* cn_broker_create(const struct CnLoadVt* vp)
 * @brief *** todo ***.
 * @param[in] vp Input.
 * @returns pointer to CnBroker instance.
 */
CnBroker* cn_broker_create(const struct CnLoadVt* vp);

/**
 * @fn void cn_broker_destroy(CnBroker* broker)
 * @brief *** todo ***.
 * @param[in] broker Input.
 */
void cn_broker_destroy(CnBroker* broker);

/**
 * @fn CnChannel* cn_broker_search(CnBroker* broker, const char* topic)
 * @brief *** todo ***.
 * @param[in,out] broker Input/output.
 * @param[in] topic Input.
 * @returns pointer to CnChannel instance.
 */
CnChannel* cn_broker_search(CnBroker* broker, const char* topic);

/**
 * @fn const char* cn_channel_gettopic(const CnChannel* ch)
 * @brief *** todo ***.
 * @param[in] ch Input.
 * @returns string representation of the topic.
 */
const char* cn_channel_gettopic(const CnChannel* ch);

/**
 * @fn CnSubscriber* cn_subscriber_create(CnBroker* broker)
 * @brief *** todo ***.
 * @param[in,out] broker Input/output.
 * @returns pointer to CnSubscriber instance.
 */
CnSubscriber* cn_subscriber_create(CnBroker* broker);

/**
 * @fn void cn_subscriber_destroy(CnSubscriber* sber)
 * @brief *** todo ***.
 * @param[in,out] sber Input/output.
 */
void cn_subscriber_destroy(CnSubscriber* sber);

/**
 * @fn CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch)
 * @brief *** todo ***.
 * @param[in,out] sber Input/output.
 * @param[in,out] ch Input/output.
 * @returns payload.
 */
CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch);

/**
 * @fn CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch)
 * @brief *** todo ***.
 * @param[in,out] sber Input/output.
 * @param[in,out] ch Input/output.
 * @returns payload.
 */
CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch);

/**
 * @fn void cn_subscriber_release(CnSubscriber* sber)
 * @brief *** todo ***.
 * @param[in,out] sber Input/output.
 */
void cn_subscriber_release(CnSubscriber* sber);

#endif /* CN_BROKER_H */
