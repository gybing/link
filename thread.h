#pragma once

#include <windows.h>

//#define TIME_WAIT_BM  5000				//�ȴ�����ŷ�ʱ��

DWORD CALLBACK wait_bestmove_threadproc(PVOID pvoid);			// �̻߳ص�����,���ڴ�������˼����Ϣ������ʾ
DWORD CALLBACK ThreadProcLoopMove(LPVOID lparam);
