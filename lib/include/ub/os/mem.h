#ifndef UB_OS_MEM_H
#define UB_OS_MEM_H

#include <stddef.h>

void* ub_malloc(size_t size);

void ub_free(void* ptr);

void ub_exit(void);

#endif /* UB_OS_MEM_H */
