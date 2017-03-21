#ifndef __SWT_PROCESS_H__
#define __SWT_PROCESS_H__

#include "swt_types.h"

static u_char master_process[] = "master process";
static u_char work_process[] = "work process";

static pid_t sub_pid;

void start_work(const char* filename, long i);
void start_process();

#endif    //__SWT_PROCESS_H__