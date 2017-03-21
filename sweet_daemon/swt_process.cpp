#include "swt_process.h"
#include "swt_setproctitle.h"
#include "general.h"


void start_work(const char* filename, long i)
{
	if (check_file_exists((char*)filename))
	{
		remove(filename);
	}

	FILE* pFile = fopen(filename, "a+");

	//int i = 0;
	while (true)
	{
		fprintf(pFile, "test i = %ld\n", i);
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
			swt_setproctitle(work_process);
			start_work("sub_process", sub_pid);
			break;
		}
    default:
		{
			swt_setproctitle(master_process);
			start_work("master_process", 0);
        	break;
    	}
    }
}

