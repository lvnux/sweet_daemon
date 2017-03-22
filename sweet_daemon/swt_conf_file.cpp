#include "swt_conf_file.h"
#include "general.h"
#include <libgen.h>

swt_conf_s g_swt_conf;

CSwtConfFile::CSwtConfFile()
{
	
}

CSwtConfFile::~ CSwtConfFile()
{

}

bool CSwtConfFile::IintConfFile(char* conf_file)
{
	char szPath[260] = {0};
	trim(szPath, conf_file);
	
	if (strcmp(szPath, "") == 0)
	{
		GetConfFile(szPath, 260);
	}

	printf("===%s===\n", szPath);
	
	if (!check_file_exists(szPath))
	{
		printf("file: %s does not exist\n", szPath);
		return false;
	}

	m_CfgFromXml.LoadXml(szPath);
	
	return true;
}

bool CSwtConfFile::Parse()
{
	return true;
}

char* CSwtConfFile::GetConfFile(char* path, u_int length) const
{
	char link[128] = {0};
    snprintf(link, 128, "/proc/%d/exe", getpid());
    if(-1 == readlink(link, path, length))
    {
    	strcpy(path, ".");
    }
	else
	{
		dirname(path);
		dirname(path);
	}

	strcat(path, "/conf/confing.xml");

	return path;
}

