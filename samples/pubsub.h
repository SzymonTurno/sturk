/**
 * \file pubsub.h
 * \brief Header file for pubsub sample code.
 *
 * *** todo ***.
 */

#ifndef PUBSUB_H
#define PUBSUB_H

#include "cantil/broker.h"
#include "cantil/str.h"

extern const struct CnLoadVt SAMPLE_LOAD_API[];

struct CnStrq* single_thread_pubsub(void);

struct CnStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
