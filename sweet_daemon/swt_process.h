#ifndef __SWT_PROCESS_H__
#define __SWT_PROCESS_H__

#include "swt_types.h"

static u_char master_process[] = "master process";
static u_char work_process[] = "work process";

typedef struct
{
    int     signo;   //需要处理的信号
    void  (*handler)(int signo);   //收到signo信号后就会回调handler方法
} swt_signal_t;

typedef struct 
{
    pid_t pid;
    unsigned exited:1;
} swt_process_t;

void ngx_signal_handler(int signo);

void swt_master_process();
void swt_start_worker_processes();
void start_work(const char* filename, long i);

pid_t swt_start_one_worker_process();
void swt_stop_worker_process();
void swt_reap_children();

swt_int_t swt_init_signals();
void swt_process_get_status();

#endif    //__SWT_PROCESS_H__