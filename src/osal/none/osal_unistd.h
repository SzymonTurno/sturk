#ifndef OSAL_UNISTD_H
#define OSAL_UNISTD_H

#ifdef WIN32

#include <direct.h>
#include <io.h>

#define F_OK   0
#define access _access

#else /* not defined: WIN32 */

#include <unistd.h>

#endif /* WIN32 */

#endif /* OSAL_UNISTD_H */
