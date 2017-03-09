#ifndef __LOCK_H__
#define __LOCK_H__

#include "err.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "Types.h"

class CThreadCondition;

class CThreadMutex
{
	friend class CThreadCondition;
public:
	inline CThreadMutex()
	{
		int rc = pthread_mutex_init(&m_cs, NULL);
		if (rc)
			errno_assert(rc);
	}

	inline ~CThreadMutex()
	{
		int rc = pthread_mutex_destroy(&m_cs);
		if (rc)
			errno_assert(rc);
	}

	inline void acquire()
	{
		int rc = pthread_mutex_lock(&m_cs);
		if (rc)
			errno_assert (rc);
	}

	inline void release()
	{
		int rc = pthread_mutex_unlock(&m_cs);
		if (rc)
			errno_assert (rc);
	}

private:
	pthread_mutex_t m_cs;

};

class CThreadCondition
{
public:
	CThreadCondition()
	{
		int nret = pthread_cond_init(&m_hEvent, NULL);
		errno_assert(nret == 0);
	}
	~CThreadCondition()
	{
		int nret = pthread_cond_destroy(&m_hEvent);
		errno_assert(nret == 0);
	}

public:
	void signal()
	{
		int nret = pthread_cond_signal(&m_hEvent);
		errno_assert(nret == 0);
	}

	//-1：错误，0：信号到来，1：超时
	int wait(int nTimeout)
	{
		if(nTimeout <= 0)
		{
			return pthread_cond_wait(&m_hEvent, &m_lock.m_cs);
		}
		else
		{
			struct timespec ts;
			int nret = clock_gettime(CLOCK_REALTIME, &ts);
			errno_assert(0 == nret);

			ts.tv_sec += nTimeout/1000;
			int64_t i64NanoSec = ts.tv_nsec+(nTimeout % 1000) * 1000000;
			if(i64NanoSec>=1000000000)
			{
				ts.tv_sec += i64NanoSec/1000000000;
				ts.tv_nsec = i64NanoSec%1000000000;
			}
			else
			{
				ts.tv_nsec = i64NanoSec;
			}

			ts.tv_sec += nTimeout;
			nret = pthread_cond_timedwait(&m_hEvent, &m_lock.m_cs, &ts);
			if(ETIMEDOUT == nret)
				return 1;

			return 0;
		}
	}

private:
	pthread_cond_t m_hEvent;
	CThreadMutex m_lock;

};
#endif

class CMutexGuard
{
public:
	CThreadMutex* m_pMutex;
	bool m_bLock;

public:
	CMutexGuard(CThreadMutex* pMutex, bool bLock = true) : m_pMutex(pMutex), m_bLock(bLock)
	{
		if(NULL == m_pMutex)
			return;

		if(m_bLock)
			m_pMutex->acquire();
	}
	~CMutexGuard() 
	{
		if(NULL == m_pMutex)
			return;

		if(m_bLock)
			m_pMutex->release();
	}
};

class CMutexRefGuard
{
public:
	CThreadMutex& m_mutex;
	bool m_bLock;

public:
	CMutexRefGuard(CThreadMutex& mutex, bool bLock = true) : m_mutex(mutex), m_bLock(bLock)
	{
		if(m_bLock)
			m_mutex.acquire();
	}
	~CMutexRefGuard() 
	{
		if(m_bLock)
			m_mutex.release();
	}
};

#endif
