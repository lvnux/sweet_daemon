#ifndef __SWT_CONF_FILE_H__
#define __SWT_CONF_FILE_H__

#include "swt_types.h"
#include "CfgFromXml.h"

typedef struct
{
    std::string workmode;
    std::string log_level;
}swt_sys_param;

typedef struct
{
    std::string host;
    uint16_t    port;
    uint16_t    index;
}swt_redis_param;

typedef struct 
{
    swt_sys_param* sys;
    swt_redis_param* redis;
}swt_conf_s;

extern swt_conf_s g_swt_conf;

class CSwtConfFile
{
public:
    CSwtConfFile();
    ~CSwtConfFile();

public:
    bool IintConfFile(char* conf_file);
    bool Parse();

private:
    char* GetConfFile(char* path, u_int length) const;

private:
    CCfgFromXml m_CfgFromXml;
};


#endif    //__SWT_CONF_FILE_H__