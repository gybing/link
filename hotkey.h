#pragma once

#include <windows.h>
#include <iostream>

#define ALT_F1     1
#define ALT_F6     2
#define ALT_F7     3
#define ALT_F8     4
#define ALT_F9     5
#define ALT_F10     6
#define ALT_F11     7
#define ALT_F12     8
#define CTRL_ALT_F1     9
#define CTRL_ALT_F2     10
#define CTRL_ALT_F3     11
#define CTRL_ALT_F4     12

#define HOTKEYS  12						//�ȼ�����

extern std::string kmess[HOTKEYS];		//�ȼ���ʾ��Ϣ

extern bool bAutoGo;					//�Զ����ӿ���	
extern bool bBH;						//�����Ƿ��������ӷ�������	
extern bool	bConnected;					//���߿���
extern bool bGoInfinite;				//���޷�������
extern int timeWaitBM;					//�ȴ�����ŷ���ʱʱ��

void	hotkey(void);					//ע���ȼ�
void	unhotkey(void);					//�����ȼ����ͷ���Դ
void	onhotkey(MSG msg);				//�ȼ��߼�