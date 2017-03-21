#include "swt_setproctitle.h"
#include <string.h>
#include <stdlib.h>

extern char **environ;
extern char **swt_argv; 
extern char **swt_os_argv;

static char *swt_os_argv_last;

int swt_init_setproctitle()
{
    u_char      *p;
    size_t       size;
    u_int        i;

    size = 0;

    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    p = (u_char*)malloc(size);
    if (p == NULL) {
        return -1;
    }

    swt_os_argv_last = swt_os_argv[0];

    for (i = 0; swt_os_argv[i]; i++) {
        if (swt_os_argv_last == swt_os_argv[i]) {
            swt_os_argv_last = swt_os_argv[i] + strlen(swt_os_argv[i]) + 1;
        }
    }

    for (i = 0; environ[i]; i++) {
        if (swt_os_argv_last == environ[i]) {

            size = strlen(environ[i]) + 1;
            swt_os_argv_last = environ[i] + size;

            swt_cpystrn(p, (u_char *) environ[i], size);
            environ[i] = (char *) p;
            p += size;
        }
    }

    swt_os_argv_last--;

    return 0;
}

//Linux修改进程名称(setproctitle()) .
void
swt_setproctitle(const u_char *title)
{
    u_char     *p;


    swt_os_argv[1] = NULL;

    p = swt_cpystrn((u_char *) swt_os_argv[0], (u_char *) "sweet: ",
                    swt_os_argv_last - swt_os_argv[0]);

    p = swt_cpystrn(p, (u_char *) title, swt_os_argv_last - (char *) p);

    if (swt_os_argv_last - (char *) p) {
        memset(p, SWT_SETPROCTITLE_PAD, swt_os_argv_last - (char *) p);
    }
}

