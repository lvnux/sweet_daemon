#include "Log.h"

#include <time.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>


CLog g_log;

CLog::CLog(void)
{
	m_hlog = NULL;
}

CLog::~CLog(void)
{
	if(m_hlog != NULL)
		fclose(m_hlog);
}

void CLog::InitParam(const char* loglevel, const char* pszName)
{
	getcwd(m_szFileName, MAX_PATH);

	strcat(m_szFileName, "/log/");
	mkdir(m_szFileName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	strcat(m_szFileName, pszName);
	m_hlog = NULL;
	if(stricmp(loglevel, "DEBUG") == 0)
		m_nLevel = 0;
	else if(stricmp(loglevel, "INFO") == 0)
		m_nLevel = 1;
	else if(stricmp(loglevel, "ERROR") == 0)
		m_nLevel = 2;
	else
		m_nLevel = -1;
}

bool CLog::CreateLogFile()
{
	char curdate[50] = {0};

	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	sprintf(curdate, "%d%.02d%.02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);

	CMutexRefGuard guard(m_csWrite);
	if(strcmp(m_curday, curdate) != 0)
	{
		if(m_hlog != NULL)
			fclose(m_hlog);
		m_hlog = NULL;

		strcpy(m_curday, curdate);
		char szFileName[MAX_PATH] = {0};
		sprintf(szFileName, "%s%s.log", m_szFileName, m_curday);

		int ret = check_file_exists(m_szFileName);
		if(0 == ret)
			return true;

		if((m_hlog = fopen(szFileName, "a+"))==NULL)
			return false;
	}

	return true;
}

void CLog::WriteLog(int nLevel, const char *format, ...)
{
	if(nLevel < m_nLevel)
		return;

	//打印日志级别
	char clog_level[6] = {0};
	switch (nLevel)
	{
	case 0:
		sprintf(clog_level, "%s", "DEBUG");
		break;
	case 1:
		sprintf(clog_level, "%s", "INFO ");
		break;
	case 2:
		sprintf(clog_level, "%s", "ERROR");
		break;
	default:
		sprintf(clog_level, "%s", "INFO ");
	}

	if(CreateLogFile() == false)
		return ;

	char szCurDate[9];
	char szCurTime[9];

	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	sprintf(szCurDate, "%.02d-%.02d-%.02d", p->tm_year+1900-2000, p->tm_mon+1, p->tm_mday);
	sprintf(szCurTime, "%.02d:%.02d:%.02d",p->tm_hour, p->tm_min, p->tm_sec);

	char szLog[1024] = {0};

	sprintf(szLog, "[%s %s] [%s] ", szCurDate, szCurTime, clog_level);
	va_list	vaParam;
	va_start(vaParam, format);
	vsnprintf(szLog + 28, 1000, format, vaParam);
	va_end(vaParam);

	m_csWrite.acquire();
	fprintf(m_hlog, "%s\r\n", szLog);
	printf("%s\r\n", szLog);
	fflush(m_hlog);
	m_csWrite.release();
}

void CLog::WriteWinHex(const char* pszFlag, const BYTE* pbyLogData, int nLogLen)
{
	if(m_nLevel >= 2 || nLogLen > 2048 || nLogLen <= 0)
		return;

	if(CreateLogFile() == false)
		return ;

	char szCurTime[9];

#ifdef ON_PLATFORM_WINDOWS
	_strtime(szCurTime);
#else
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	sprintf(szCurTime, "%.02d:%.02d:%.02d",p->tm_hour, p->tm_min, p->tm_sec);
#endif

	char strbuff[4] = {0};
	char strLog[8192] = {0};
	for(int i = 0; i < nLogLen; i++)
	{
		sprintf(strbuff, "%02X ", pbyLogData[i] & 0xFF);
		strcat(strLog, strbuff);
	}

	m_csWrite.acquire();
	fprintf(m_hlog, "%s (%s):%s\r\n", szCurTime, pszFlag, strLog);
	fflush(m_hlog);
	m_csWrite.release();
}

int  CLog::GetLogLevel()
{
	return m_nLevel;
}
