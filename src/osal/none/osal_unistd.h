#ifndef UB_OSAL_UNISTD_H
#define UB_OSAL_UNISTD_H

#ifdef WIN32
#include <io.h>
#include <direct.h>
#define F_OK 0
#define access _access
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#endif /* UB_OSAL_UNISTD_H */
