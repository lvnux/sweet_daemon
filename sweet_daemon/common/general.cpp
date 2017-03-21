#include "general.h"

bool check_file_exists(char* pszfilename)
{
	return (0 == access(pszfilename, F_OK));
}

char* get_cur_directory(char* szpath, int nlen)
{
	return getcwd(szpath, nlen);
}

char* ltrim(char* dst, char* src)
{
	int i = 0;

	while(src[i] == ' ')
		++i;

	strcpy(dst, src + i);
	
	return dst;
}

char* rtrim(char* dst, char* src)
{
	int i = strlen(src) - 1;

	while(src[i] == ' ')
		--i;

	strncpy(dst, src, i+1);

	return dst;
}


char* trim(char* dst, char* src)
{
	int i, j ;

	i = 0;

	j = strlen(src) - 1;

	while(src[i] == ' ')
		++i;

	while(src[j] == ' ')
		--j;

	strncpy(dst, src + i , j - i + 1);
	dst[j - i + 1] = '\0';

	return dst;
}

