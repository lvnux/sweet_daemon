#ifndef __TYPYS_H__
#define __TYPES_H__

#ifndef Byte
typedef unsigned char Byte;
#endif

#ifndef u_char
typedef unsigned char u_char;
#endif

#ifndef u_int
typedef unsigned int u_int;
#endif

#ifndef ulong_t
typedef unsigned long ulong_t;
#endif

#ifdef _MSC_VER

#ifndef int8_t
typedef __int8 int8_t;
#endif
#ifndef int16_t
typedef __int16 int16_t;
#endif
#ifndef int32_t
typedef __int32 int32_t;
#endif
#ifndef int64_t
typedef __int64 int64_t;
#endif
#ifndef uint8_t
typedef unsigned __int8 uint8_t;
#endif
#ifndef uint16_t
typedef unsigned __int16 uint16_t;
#endif
#ifndef uint32_t
typedef unsigned __int32 uint32_t;
#endif
#ifndef uint64_t
typedef unsigned __int64 uint64_t;
#endif


#else
#include <stdint.h>
#endif

#endif