#include "swt_process.h"
#include "swt_setproctitle.h"
#include "general.h"
#include "swt_conf_file.h"
#include "swt_common_func.h"
#include <sys/types.h>
#include <sys/wait.h>

extern swt_conf_s g_swt_conf;
extern CSwtConfFile g_swtConfFile;

pid_t swt_pid;

sig_atomic_t  swt_reap;
sig_atomic_t  swt_terminate;   //TERM信号
sig_atomic_t  swt_quit;   //QUIT信号
swt_uint_t    swt_exiting; //ngx_exiting标志位仅由ngx_worker_process_cycle方法在退出时作为标志位使用
sig_atomic_t  swt_reconfigure;  //HUP信号

swt_process_t swt_processes[1024];    //用于存储子进程信息
uint32_t swt_last_process;  //子进程个数

static const char* szfilename = "swt_test_sub.txt";

swt_signal_t  signals[] = {
    {SIGHUP, ngx_signal_handler},
    {SIGQUIT, ngx_signal_handler},
    {SIGTERM, ngx_signal_handler},
    {SIGALRM, ngx_signal_handler},
    {SIGINT, ngx_signal_handler},
    {SIGIO, ngx_signal_handler},
    {SIGCHLD, ngx_signal_handler},
    {SIGSYS, SIG_IGN},
    {SIGPIPE, SIG_IGN},
    {0, NULL}
};

void swt_master_process()
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGALRM);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGIO);
	sigaddset(&set, SIGCHLD);

	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
	{
		printf("sigprocmask failed\n");
		return;
	}

	sigemptyset(&set);

	swt_setproctitle(master_process);
	
	swt_start_worker_processes();

	for ( ;; )
	{
		sigsuspend(&set);
		if (swt_terminate)
		{
			swt_stop_worker_process();

			if (check_file_exists((char*)szfilename))
			{
				remove(szfilename);
			}
			
			exit(0);
		}
		
		if (swt_reap)
		{
			swt_reap = 0;
			swt_reap_children();
		}

		if (swt_reconfigure)
		{
			swt_reconfigure = 0;
			swt_stop_worker_process();
			g_swtConfFile.Parse();
			swt_start_worker_processes();
		}

		sleep(1);
	}
	
}

void swt_start_worker_processes()
{
	pid_t pid;
	
	swt_last_process = g_swt_conf.worker.processes;
	
	for (uint32_t i = 0; i < swt_last_process; i++)
	{
		pid = swt_start_one_worker_process();
		swt_processes[i].pid = pid;
		swt_processes[i].exited = 0;
	}
}

pid_t swt_start_one_worker_process()
{
	pid_t pid;
	pid = fork();
	switch (pid)
	{
		case -1:
			start_work("swt_test_sub.error", (long)9);
			break;
		case 0:
			swt_pid = getpid();
			start_work(szfilename, (long)swt_pid);
			break;
		default:
			break;
	}

	return pid;
}

void swt_stop_worker_process()
{
	for (uint32_t i = 0; i < swt_last_process; i++)
	{
		kill(swt_processes[i].pid, SIGTERM);
		swt_processes[i].exited = 1;
	}

	for (uint32_t i = 0; i < swt_last_process; i++)
	{
		waitpid(swt_processes[i].pid, NULL, 0);
	}
}

void swt_reap_children()
{
	pid_t pid;
	for (uint32_t i = 0; i < swt_last_process; i++)
	{
		if (1 == swt_processes[i].exited)
		{
			waitpid(swt_processes[i].pid, NULL, 0);
			pid = swt_start_one_worker_process();
			swt_processes[i].pid = pid;
			swt_processes[i].exited = 0;
		}
	}
}

void start_work(const char* filename, long i)
{
	sigset_t set;

	swt_setproctitle(work_process);

	sigemptyset(&set);
	if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
	{
		printf("work sigprocmask failed\n");
		return;
	}
	
	/*
	if (check_file_exists((char*)filename))
	{
		remove(filename);
	}
	*/

	FILE* pFile = fopen(filename, "a+");

	while (true)
	{
		if (swt_terminate)
		{
			exit(0);
		}
		
		fprintf(pFile, "pid = %ld, g_swt_conf: %s, %s, %d, %d\n", 
			i, g_swt_conf.sys.workmode.c_str(), g_swt_conf.redis.host.c_str(), g_swt_conf.redis.port, swt_last_process);
		fflush(pFile);
		//++i;
		sleep(2);
	}

	if (NULL != pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
}


swt_int_t
swt_init_signals()
{
	swt_signal_t* sig;
	struct sigaction sa;

	for (sig = signals; sig->signo != 0; sig++)
	{
		memset(&sa, 0, sizeof(struct sigaction));
		sa.sa_handler = sig->handler;
		sigemptyset(&sa.sa_mask);

		if (sigaction(sig->signo, &sa, NULL) == -1)
		{
			swt_write_stderr("sigaction error");
			return SWT_FAILURE;
		}
	}

	return SWT_SUCCESS;
}


void ngx_signal_handler(int signo)
{
    swt_signal_t    *sig;

    for (sig = signals; sig->signo != 0; sig++)
	{
        if (sig->signo == signo) {
            break;
        }
    }

	switch (signo)
	{
		case SIGHUP:
		{
			swt_reconfigure = 1;
			break;
		}
		case SIGQUIT:
		{
			break;
		}
		case SIGALRM:
		{
			break;
		}
		case SIGTERM:
		case SIGINT:
		{
			swt_terminate = 1;
			break;
		}
		case SIGIO:
		{
			break;
		}
		case SIGCHLD:
		{
			swt_reap = 1;
			break;
		}
		default:
		{
			break;
		}
	}

	if (signo == SIGCHLD) { //回收子进程资源waitpid
        swt_process_get_status();
    }
}

void swt_process_get_status()
{
	pid_t pid;

	for ( ;; )
	{
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid == 0)    //没有子进程退出
		{
			return;
		}

		if (pid == -1)    //waitpid执行异常
		{
			return;
		}

		for (uint32_t i = 0; i < swt_last_process; i++)
		{
			if (swt_processes[i].pid == pid)
			{
				swt_processes[i].exited = 1;
				break;
			}
		}
	}
}

