#ifndef OSAL_UNISTD_H
#define OSAL_UNISTD_H

#ifdef WIN32
#include <direct.h>
#include <io.h>
#define F_OK   0
#define access _access
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#endif /* OSAL_UNISTD_H */
