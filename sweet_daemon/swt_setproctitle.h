#ifndef __SWT_SETPROCTITLE_H__
#define __SWT_SETPROCTITLE_H__

#include "swt_string.h"

#define SWT_SETPROCTITLE_PAD       '\0'

int swt_init_setproctitle();
void swt_setproctitle(const u_char *title);


#endif //__SWT_SETPROCTITLE_H__