/**
 * @file cn/broker.h
 * @brief Header file for message broker.
 *
 * *** todo ***.
 */

#ifndef CN_BROKER_H
#define CN_BROKER_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef char CnLoad
 * @brief Payload.
 */
typedef char CnLoad;

/**
 * @struct CnLoadVt
 * @brief Vtable for payload construction.
 *
 * Members:
 * - size,
 * - ctor,
 * - dtor.
 */
struct CnLoadVt {
	/**
	 * @fn size_t (*size)(void)
	 * @brief Callback for obtaining the size.
	 *
	 * @returns number of bytes required to allocate the payload.
	 * @see cn_broker_create()
	 *
	 * Callback for computing memory space required to allocate the
	 * payload. It is called only once - when invoking cn_broker_create().
	 */
	size_t (*size)(void);

	/**
	 * @fn void (*ctor)(CnLoad*, va_list)
	 * @brief Constructor callback of the payload.
	 *
	 * @param[in,out] payload pointer.
	 * @param[in] list of arguments (except the first argument) passed to cn_publish().
	 * @see cn_publish()
	 */
	void (*ctor)(CnLoad*, va_list);

	/**
	 * @fn void (*dtor)(CnLoad*)
	 * @brief Destructor callback of the payload.
	 *
	 * @param[in,out] payload pointer.
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
 * @param[in] ... list of arguments used by CnLoadVt.
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
