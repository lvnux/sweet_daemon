#ifndef __SWT_LOG_H__
#define __SWT_LOG_H__

#include "Lock.h"
#include <stdio.h>

class CLog
{
#define MAX_PATH 260
public:
	CLog(void);
	~CLog(void);

public:
	void InitParam(const char* loglevel, const char* pszName);
	int  GetLogLevel();
	void WriteLog(int nLevel, const char *format, ...);
	void WriteWinHex(const char* pszFlag, const BYTE* pbyLogData, int nLogLen);

private:
	bool CreateLogFile();

private:
	char m_szFileName[MAX_PATH];
	FILE* m_hlog;
	char m_curday[50];
	int m_nLevel;
	CThreadMutex m_csWrite;

};

extern CLog g_log;

#define log_debug(format, ...) g_log.WriteLog(0, format, ##__VA_ARGS__)
#define log_info(format, ...) g_log.WriteLog(1, format, ##__VA_ARGS__)
#define log_err(format, ...) g_log.WriteLog(2, format, ##__VA_ARGS__)
#define log_hex(pszFlag, pbyLogData, nLogLen) g_log.WriteWinHex(pszFlag, pbyLogData, nLogLen)

#endif  //__SWT_LOG_H__