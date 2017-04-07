#include "swt_daemon.h"

extern pid_t swt_pid;

swt_int_t swt_daemon()
{
    switch (fork()) {
    case -1:
        printf("fork() failed\n");
        return SWT_FAILURE;

    case 0:
        break;

    default:
        exit(0);
    }

	swt_pid = getpid();
		
    if (setsid() == -1) {
        printf("setsid() failed\n");
        return SWT_FAILURE;
    }

    umask(0);

	int fd;
    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        printf("open(\"/dev/null\") failed\n");
        return SWT_FAILURE;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        printf("dup2(STDIN) failed\n");
        return SWT_FAILURE;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        printf("dup2(STDOUT) failed\n");
        return SWT_FAILURE;
    }

#if 0
    if (dup2(fd, STDERR_FILENO) == -1) {
        printf("dup2(STDERR) failed\n");
        return SWT_FAILURE;
    }
#endif

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            printf("close() failed\n");
            return SWT_FAILURE;
        }
    }

    return SWT_SUCCESS;
}


