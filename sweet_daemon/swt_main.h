#ifndef __SWT_MAIN_H__
#define __SWT_MAIN_H__

#include "swt_common_func.h"
#include "swt_string.h"
#include "swt_setproctitle.h"
#include "swt_process.h"

#define SWEET_VERSION "1.0.0"

#define SWT_CONF_PATH "../conf/sweet.xml"

static int option_show_help = 0;
static int option_show_version = 0;
static int option_start_work = 1;        //默认启动工作

static char      *swt_conf_file = "";

int              swt_argc;
char           **swt_argv; 
char           **swt_os_argv;


static swt_int_t swt_save_argv(int argc, char *const *argv);
static swt_int_t swt_get_options(int argc, char *const *argv);
static inline void swt_show_version();
static inline void swt_show_help();

#endif    //__SWT_MAIN_H__