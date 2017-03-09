#ifndef __ERRINFO_H__
#define __ERRINFO_H__

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

#ifdef __GNUC__
#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

inline void net_abort(const char *errmsg_)
{
	abort();
}

#define net_assert(x) \
	do {\
	if(unlikely(!(x))) {\
	fprintf(stderr, "Assertion failed: %s(%s:%d)\n", #x, \
	__FILE__, __LINE__);\
	net_abort(#x);\
	}\
	} while(false) 

#define alloc_assert(x) \
	do {\
	if(unlikely(!x)) {\
	fprintf(stderr, "FATAL ERROR: OUT OF MEMORY(%s:%d)\n",\
	__FILE__, __LINE__);\
	net_abort("FATAL ERROR: OUT OF MEMORY");\
	}\
	} while(false)

#define errno_assert(x) \
	do {\
	if(unlikely(!(x))) {\
	const char *errstr = strerror(errno);\
	fprintf(stderr, "%s(%s:%d)\n", errstr, __FILE__, __LINE__);\
	net_abort(errstr);\
	}\
	} while(false)


#endif
