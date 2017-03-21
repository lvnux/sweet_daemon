#ifndef __GENERAL_H__
#define __GENERAL_H__


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>


//π§æﬂ¿‡
bool check_file_exists(char* pszfilename);
char* get_cur_directory(char* szpath, int nlen);

char* ltrim(char* dst, char* src);
char* rtrim(char* dst, char* src);
char* trim(char* dst, char* src);

#endif
