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
	GetValue("sys", g_swt_conf.sys);
	GetValue("redis", g_swt_conf.redis);
	
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


bool CSwtConfFile::GetValue(const char* element_name, swt_sys_param& sys_param)
{
	if(false == m_CfgFromXml.MoveRootElement() || false == m_CfgFromXml.MoveToElement(element_name))
	{
		return false;
	}

	m_CfgFromXml.GetTextValue("workmode", sys_param.workmode);
	m_CfgFromXml.GetTextValue("log-level", sys_param.log_level);

	return true;
}

bool CSwtConfFile::GetValue(const char* element_name, swt_redis_param& redis_param)
{
	if(false == m_CfgFromXml.MoveRootElement() || false == m_CfgFromXml.MoveToElement(element_name))
	{
		return false;
	}

	m_CfgFromXml.GetTextValue("host", redis_param.host);
	m_CfgFromXml.GetTextValue("port", redis_param.port);
	m_CfgFromXml.GetTextValue("index", redis_param.index);

	return true;
}

