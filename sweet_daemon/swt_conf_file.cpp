#include "swt_conf_file.h"
#include "general.h"
#include <libgen.h>

swt_conf_s g_swt_conf;
CSwtConfFile g_swtConfFile;

CSwtConfFile::CSwtConfFile()
{
	
}

CSwtConfFile::~ CSwtConfFile()
{

}

bool CSwtConfFile::IintConfFile(char* conf_file)
{
	trim(m_szCfgPath, conf_file);
	
	if (strcmp(m_szCfgPath, "") == 0)
	{
		GetConfFile(m_szCfgPath, 260);
	}

	printf("===%s===\n", m_szCfgPath);
	
	if (!check_file_exists(m_szCfgPath))
	{
		printf("file: %s does not exist\n", m_szCfgPath);
		return false;
	}
	
	return Parse();
}

bool CSwtConfFile::Parse()
{
	m_CfgFromXml.LoadXml(m_szCfgPath);
	
	GetValue("sys", g_swt_conf.sys);
	GetValue("redis", g_swt_conf.redis);
	GetValue("worker", g_swt_conf.worker);
	
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

bool CSwtConfFile::GetValue(const char* element_name, swt_worker& worker)
{
	if(false == m_CfgFromXml.MoveRootElement() || false == m_CfgFromXml.MoveToElement(element_name))
	{
		return false;
	}

	m_CfgFromXml.GetTextValue("processes", worker.processes);

	return true;
}

