#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>

#include "swt_string.h"
#include "swt_setproctitle.h"

static pid_t master_pid;
static pid_t sub_pid;

static int option_show_help = 0;
static int option_show_version = 0;
static int option_in_swt = 0;
static int option_start_work = 0;


int              swt_argc;
char           **swt_argv; 
char           **swt_os_argv;


static swt_int_t swt_save_argv(int argc, char *const *argv);
static swt_int_t swt_get_options(int argc, char *const *argv);
static inline void swt_show_version();
static inline void swt_show_help();
static swt_int_t swt_daemon();



void start_work(const char* filename, long i)
{
	if (access(filename, 0) != -1)
	{
		remove(filename);
	}

	FILE* pFile = fopen(filename, "a+");

	//int i = 0;
	while (true)
	{
		fprintf(pFile, "test i = %d\n", i);
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

void start_process()
{
	switch (fork()) {
    case -1:
        printf("fork() failed\n");
        return;

    case 0:
		{
        	sub_pid = getpid();
			printf("sub pid: %ld\n", (long)sub_pid);
			swt_setproctitle("sub swt process");
			start_work("sub_process", sub_pid);
			break;
		}
    default:
		{
			swt_setproctitle("test_swt process");
			start_work("master_process", master_pid);
        	break;
    	}
    }
}


int main(int argc, char** argv)
{
	if (SWT_SUCCESS != swt_get_options(argc, argv))
		return SWT_FAILURE;

	swt_save_argv(argc, argv);
	swt_init_setproctitle();
	
    if (1 == argc)
    {
        printf("start direct\n");
		swt_setproctitle("test_swt process");
		start_work("single_pattern", 0);
        return 0;
    }
	
	master_pid = getpid();

	if (option_show_help)
	{
		swt_show_help();
	}

	if (option_show_version)
	{
		swt_show_version();
	}

	if (option_start_work && option_in_swt)
	{
    	swt_daemon();
		start_process();
	}
	
    return 0;
}


static swt_int_t swt_get_options(int argc, char *const *argv)
{
	int oc;
    while ((oc = getopt(argc, argv, ":dhvs:")) != -1)
    {
        switch (oc)
        {
        	case 'h':
			{
				option_show_help = 1;
				break;
			}
            case 'd':
            {
                printf("start in swt\n");
				option_in_swt = 1;
				option_start_work = 1;
                break;
            }
            case 'v':
            {
                option_show_version = 1;
                break;
            }
            case 's':
            {
                printf("param is: %s\n", optarg);
                break;
            }
            case '?':
            {
                printf("Uage: %s [-hdv] [-s arg] arg \n", argv[0]);
                break;
            }
            case ':':
            {
                printf("absence param\n");
                break;
            }
        }
    }

	return SWT_SUCCESS;
}


static inline void swt_show_version()
{
	swt_write_stdout("sweet version: " SWEET_VERSION SWT_LINEFEED);
}


static inline void swt_show_help()
{
	swt_write_stdout(
		"Usage: nginx [-?hvVtTq] [-s signal] [-c filename] "
                             "[-p prefix] [-g directives]" SWT_LINEFEED
        "Options:" SWT_LINEFEED
        "  -?,-h         : this help" SWT_LINEFEED
        "  -v            : show version and exit" SWT_LINEFEED
        "  -d            : start worker in daemon" SWT_LINEFEED
        "  -s signal     : send signal to a master process: " SWT_LINEFEED
        "                  stop, quit, reopen, reload" SWT_LINEFEED
        "  -c filename   : set configuration file (default: xxxxx.conf)" SWT_LINEFEED
	);
}


static int swt_save_argv(int argc, char *const *argv)
{
    size_t     len;
    int  i;

    swt_os_argv = (char **) argv;
    swt_argc = argc;

    swt_argv = (char**)malloc((argc + 1) * sizeof(char *));
    if (swt_argv == NULL) {
        return -1;
    }

    for (i = 0; i < argc; i++) {
        len = strlen(argv[i]) + 1;

        swt_argv[i] = (char*)malloc(len);
        if (swt_argv[i] == NULL) {
            return -1;
        }

        (void) swt_cpystrn((u_char *) swt_argv[i], (u_char *) argv[i], len);
    }

    swt_argv[i] = NULL;

    return SWT_SUCCESS;
}


static swt_int_t swt_daemon()
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

    master_pid = getpid();

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

