#ifndef __SWT_SETPROCTITLE_H__
#define __SWT_SETPROCTITLE_H__

#include "swt_types.h"

#define NGX_SETPROCTITLE_USES_ENV  1
#define NGX_SETPROCTITLE_PAD       '\0'

int swt_init_setproctitle();
void swt_setproctitle(const char *title);


#endif //__SWT_SETPROCTITLE_H__