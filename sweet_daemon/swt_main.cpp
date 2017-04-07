#include "swt_main.h"
#include "swt_conf_file.h"
#include "swt_daemon.h"

extern swt_conf_s g_swt_conf;
extern pid_t swt_pid;

int main(int argc, char** argv)
{
	if (SWT_SUCCESS != swt_get_options(argc, argv))
		return SWT_FAILURE;

	CSwtConfFile swtConfFile;
	if (false == swtConfFile.IintConfFile(swt_conf_file))
		return SWT_FAILURE;

	if (false == swtConfFile.Parse())
		return SWT_FAILURE;

	printf("g_swt_conf: %s, %s, %d, %d\n", 
		g_swt_conf.sys.workmode.c_str(), g_swt_conf.redis.host.c_str(), g_swt_conf.redis.port, g_swt_conf.worker.processes);
	
	swt_save_argv(argc, argv);
	swt_init_setproctitle();
	
	swt_pid = getpid();

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
		swt_init_signals();
    	swt_daemon();
		swt_master_process();
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
				option_start_work = 0;
				break;
			}
            case 'v':
            {
                option_show_version = 1;
				option_start_work = 0;
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

