//example:
//void MyFunc()
//{
//	//���뺯����_lock�����������ù��캯��������
//	CLockRegion _lock(&lock);
//
//	//do something
//
//	//�����˳���_lock������������
//}

#pragma once

#include <windows.h>

class CLock
{
public:
	CLock(void)	{InitializeCriticalSection(&cs);}

	~CLock(void){DeleteCriticalSection(&cs);}

	inline void Lock()	{EnterCriticalSection(&cs);	}

	inline void UnLock(){LeaveCriticalSection(&cs);	}

private:
	CRITICAL_SECTION cs;
};

class CLockRegion
{
public:
	CLockRegion(CLock* lock) : m_lock(lock)	{ m_lock->Lock();}
	~CLockRegion()	{m_lock->UnLock();	}

private:
	CLock* m_lock;
};

extern CLock lock;
