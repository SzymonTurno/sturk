#ifndef MESSAGE_H
#define MESSAGE_H

#include "ub/os/mem.h"
#include "ub/os/sysio.h"

#define LOG_MAX_LENGTH 256

static inline const char* msg_lvl(enum UBlogLvl lvl)
{
	switch (lvl) {
	case UB_DEBUG:
		return "debug";
	case UB_INFO:
		return "info";
	case UB_WARNING:
		return "warning";
	case UB_ERROR:
		return "error";
	default:
		break;
	}
	return NULL;
}

static inline char* msg_create(enum UBlogLvl lvl, const char* tag,
	const char* s1, const char* s2)
{
	char* msg = ub_malloc(LOG_MAX_LENGTH);

	if (tag)
		ub_snprintf(msg, LOG_MAX_LENGTH, "[%s][%s] %s%s\n",
			msg_lvl(lvl), tag, s1, s2);
	else
		ub_snprintf(msg, LOG_MAX_LENGTH, "[%s] %s%s\n", msg_lvl(lvl),
			s1, s2);
	return msg;
}

#endif /* MESSAGE_H */
