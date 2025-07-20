/**
 * @file cn/broker.h
 *
 * @brief Message broker.
 *
 * This file defines data types and functions that implement the
 * publish-subscribe messaging pattern. Design choices of using:
 * 1. BST - as oppose to hash table - for implementing topics dictionary,
 * 2. memory pools for messages and queue entries,
 * attempt to make this suitable for memory constrained systems.
 *
 * The memory for each message is allocated by the broker in two ways:
 * 1. directly - always, let's call memory allocated this way "static space";
 * 2. indirectly - optional, let's call memory allocated this way "dynamic space".
 *
 * The "static space" is kept within a memory pool. The size of the static space
 * is constant for all messages and it is computed from the value returned by
 * the size callback - CnLoadVt::size. Static space cannot be 0 even if the size
 * callback returns 0 as the message contains also metadata needed for thread
 * synchronization and a reference to the channel source of the message. Static
 * space contains also empty bytes for alignment - size of the static space is
 * always a multiple of the size of the metadata.
 *
 * STATIC SPACE
 * +----------+------+ <-- The load pointer (CnLoad*) points here.
 * | msg[0]   |      |     @see CnLoadVt::ctor
 * +----------+      |     @see CnLoadVt::dtor
 * | ...      | load |     @see cn_subscriber_await()
 * +----------+      |     @see cn_subscriber_poll()
 * | msg[n-1] |      |
 * +----------+------+
 * | msg[n]   | meta |
 * +----------+------+
 *
 * The "dynamic space" is anything allocated by the contructor - CnLoadVt::ctor.
 * Must be freed by the destructor - CnLoadVt::dtor. This space is optional, if
 * the constructor only initializes the message (fills the load) and does not
 * allocate memory or access global resources, destructor can be empty.
 */

#ifndef CN_BROKER_H
#define CN_BROKER_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef char CnLoad
 *
 * @brief Opaque type for the load.
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
	 * "dynamic space") and initialize the message - read arguments from
	 * va_list and fill the load passed through CnLoad pointer.
	 *
	 * @note The input va_list will hold all values passed to cn_publish()
	 * after the channel argument.
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
 * @brief *** todo ***.
 */
typedef struct CnBroker CnBroker;

/**
 * @var typedef struct CnSubscriber CnSubscriber
 * @brief *** todo ***.
 */
typedef struct CnSubscriber CnSubscriber;

/**
 * @var typedef struct CnChannel CnChannel
 * @brief *** todo ***.
 */
typedef struct CnChannel CnChannel;

/**
 * @fn void cn_publish(CnChannel* ch, ...)
 * @brief *** todo ***.
 * @param[in,out] ch Input/output.
 * @param[in] ... list of arguments used by CnLoadVt::ctor.
 * @see CnLoadVt::ctor
 */
void cn_publish(CnChannel* ch, ...);

/**
 * @fn void cn_subscribe(CnSubscriber* sber, const char* topic)
 * @brief *** todo ***.
 * @param[in,out] sber Input/output.
 * @param[in] topic Input.
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
