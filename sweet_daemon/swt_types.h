#ifndef __SWT_TYPE_H__
#define __SWT_TYPE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <string.h>

#include "Types.h"


#define SWT_SUCCESS    0
#define SWT_FAILURE    -1


#ifndef swt_int_t
typedef int swt_int_t;
#endif

#endif    //__SWT_TYPE_H__