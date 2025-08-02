/**
 * @file cantil/os/sem.h
 *
 * @see cn/os/sem.h.
 */

#ifndef CANTIL_OS_SEM_H
#define CANTIL_OS_SEM_H

#include "cn/os/sem.h"

/** @see cn_sem_create() */
#define sem_create cn_sem_create

/** @see cn_sem_destroy() */
#define sem_destroy cn_sem_destroy

/** @see cn_sem_wait() */
#define sem_wait cn_sem_wait

/** @see cn_sem_trywait() */
#define sem_trywait cn_sem_trywait

/** @see cn_sem_post() */
#define sem_post cn_sem_post

#endif /* CANTIL_OS_SEM_H */
