#include "swt_main.h"
#include "swt_conf_file.h"

extern swt_conf_s g_swt_conf;

int main(int argc, char** argv)
{
	if (SWT_SUCCESS != swt_get_options(argc, argv))
		return SWT_FAILURE;

	CSwtConfFile swtConfFile;
	if (false == swtConfFile.IintConfFile(swt_conf_file))
		return SWT_FAILURE;

	if (false == swtConfFile.Parse())
		return SWT_FAILURE;

	printf("g_swt_conf: %s, %s, %d\n", g_swt_conf.sys.workmode.c_str(), g_swt_conf.redis.host.c_str(), g_swt_conf.redis.port);
	
	swt_save_argv(argc, argv);
	swt_init_setproctitle();
	
	master_pid = getpid();

	if (option_show_help)
	{
		swt_show_help();
	}

	if (option_show_version)
	{
		swt_show_version();
	}

	if (option_start_work)
	{
    	swt_daemon();
		start_process();
	}
	
    return 0;
}


static swt_int_t swt_get_options(int argc, char *const *argv)
{
	int oc;
    while ((oc = getopt(argc, argv, ":dhvc:s:")) != -1)
    {
        switch (oc)
        {
        	case 'h':
			{
				option_show_help = 1;
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
			case 'c':
            {
                printf("param is: %s\n", optarg);
				swt_conf_file = optarg;
				option_start_work = 1;
                break;
            }
            case '?':
            {
                printf("Uage: %s [-hdv] [-s arg] arg \n", argv[0]);
				option_start_work = 0;
                break;
            }
            case ':':
            {
                printf("absence param\n");
				option_start_work = 0;
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
        "  -c filename   : set configuration file (default: "
                                   SWT_CONF_PATH ")" SWT_LINEFEED
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

