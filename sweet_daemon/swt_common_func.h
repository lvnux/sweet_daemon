#ifndef __SWT_COMMON_FUNC_H__
#define __SWT_COMMON_FUNC_H__

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define SWT_LINEFEED             "\x0a"

typedef int                      swt_fd_t;


static inline ssize_t
swt_write_fd(swt_fd_t fd, void *buf, size_t n)
{
    return write(fd, buf, n);
}


static inline void
swt_write_stderr(char *text)
{
    (void) swt_write_fd(STDERR_FILENO, text, strlen(text));
}


static inline void
swt_write_stdout(char *text)
{
    (void) swt_write_fd(STDOUT_FILENO, text, strlen(text));
}

#endif    //__SWT_COMMON_FUNC_H__