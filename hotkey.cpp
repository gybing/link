#include <windows.h>
#include <Shlwapi.h>

#include "hotkey.h"
#include "linker.h"
#include "lock.h"
#include "ucci.h"
//#include "..\ncch\file.h"

extern HANDLE  hWaitForBestmove;
extern HANDLE	hLoopMove;

bool	bAutoGo = false;						//�Զ����ӿ���
bool	bBH = true;								//�����Ƿ��������ӷ�������	
bool	bConnected = false;						//���߿���	
bool    bGoInfinite = false;					//���޷�������

std::string kmess[HOTKEYS] = {					//�ȼ���ʾ��Ϣ
	"ALT_F1 �������ӿ��ƿ���!",
	"ALT_F6 �Ͽ�����!" ,
	"ALT_F7 ���Ӳ��ԣ���ӡ���̡�ͬʱ�л���ǰ˼��ģʽ��ʱ������ȣ�! �л���Ļ��ͼλ�24λ��32λ��",
	"ALT_F8 ��ʼ������(��Ļ���ļ���ʽ)�����淽��!",
	"ALT_F9 �������ӿ��ƿ���!",
	"ALT_F10 ���뷽��!",
	"ALT_F11 �ͻ����Զ����ӱ�־!",
	"ALT_F12 �������޷���ģʽ",
	"CTRL_ALT_F1 ��ѭ���޸Ĳ���!",
	"CTRL_ALT_F2 �����ķ���������������С��ʵʱ�Ӵ�С����ֵ ��˼��ʱ�䡢��ȵȣ�",
	"CTRL_ALT_F3 �����ӷ������ƿ���",
	"CTRL_ALT_F4 ������ֹͣ˼��������������",
};

#pragma comment( lib,"shlwapi.lib")
	
//ע��HotKey��
void hotkey(void)
{
	if (RegisterHotKey(NULL, ALT_F1, MOD_ALT | MOD_NOREPEAT, VK_F1))	std::cout << ((kmess[ALT_F1-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F6, MOD_ALT | MOD_NOREPEAT, VK_F6))	std::cout << ((kmess[ALT_F6-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F7, MOD_ALT | MOD_NOREPEAT, VK_F7))	std::cout << ((kmess[ALT_F7-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F8, MOD_ALT | MOD_NOREPEAT, VK_F8))	std::cout << ((kmess[ALT_F8-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F9, MOD_ALT | MOD_NOREPEAT, VK_F9))	std::cout << ((kmess[ALT_F9-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F10, MOD_ALT | MOD_NOREPEAT, VK_F10))	std::cout << ((kmess[ALT_F10-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F11, MOD_ALT | MOD_NOREPEAT, VK_F11))	std::cout << ((kmess[ALT_F11-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, ALT_F12, MOD_ALT | MOD_NOREPEAT, VK_F12))	std::cout << ((kmess[ALT_F12-1]).c_str()) << std::endl;

	if (RegisterHotKey(NULL, CTRL_ALT_F1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_F1))	std::cout << ((kmess[CTRL_ALT_F1-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, CTRL_ALT_F2, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_F2))	std::cout << ((kmess[CTRL_ALT_F2-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, CTRL_ALT_F3, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_F3))	std::cout << ((kmess[CTRL_ALT_F3-1]).c_str()) << std::endl;
	if (RegisterHotKey(NULL, CTRL_ALT_F4, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_F4))	std::cout << ((kmess[CTRL_ALT_F4-1]).c_str()) << std::endl;
}

//ע��HotKey, �ͷ���Դ��
void unhotkey(void)
{
	::UnregisterHotKey(NULL, ALT_F1);
	::UnregisterHotKey(NULL, ALT_F6);
	::UnregisterHotKey(NULL, ALT_F7);
	::UnregisterHotKey(NULL, ALT_F8);	
	::UnregisterHotKey(NULL, ALT_F9);	
	::UnregisterHotKey(NULL, ALT_F10);
	::UnregisterHotKey(NULL, ALT_F11);
	::UnregisterHotKey(NULL, ALT_F12);

	::UnregisterHotKey(NULL, CTRL_ALT_F1);
	::UnregisterHotKey(NULL, CTRL_ALT_F2);
	::UnregisterHotKey(NULL, CTRL_ALT_F3);
	::UnregisterHotKey(NULL, CTRL_ALT_F4);
}

extern HWND hwndConsole;
void onhotkey(MSG msg)
{
    std::string input;
	int width_init, width_real;
	double threshold;
	switch(msg.wParam)
	{
	case ALT_F1:	
	case ALT_F9:	
		if(link.isready())
		{
			link.fenold="";
			link.sideToMove = (msg.wParam==ALT_F1)? (link.local_color()):(!link.local_color());					//����ʱ�ֵ��Է���Զ�̣�������
			bConnected = true;
			std::cout << "�����ѿ�ʼ!\n" << (link.sideToMove==link.local_color()?"�ֵ��ң��£���":"�ֵ�Զ���ϣ���") << std::endl;
		}
		else
		{
			std::cout << "����δ׼���ã���δ�����ͻ��ˣ� ALT_F10�����뷽����" << std::endl;
		}
		break;

	case ALT_F6:	
		if(bConnected)
		{
			bConnected = false;
			//SuspendThread(hLoopMove);				//��ͣ�߳�
			//SuspendThread(hWaitForBestmove);		//��ͣ�߳�
			ucci.StopEngine();
			std::cout << "�ѶϿ����ӣ�����ֹͣ˼����" << std::endl;
		}
		else
		{
			std::cout << "����δ���ӣ�����Ͽ���" << std::endl;
		}
		break;

	case ALT_F7:	
		if(bConnected)
		{
			CLockRegion _lock(&lock);				//_lock�����������ù��캯���������������ٽ���
			link.update_fen();
			std::cout << link.print() << std::endl;				//��ʾ��ǰ����
			std::cout << "���Ӳ��Խ������" << std::endl;		//��ʾ��ǰ����
			ucci.model=1-ucci.model;							//�л�˼��ģʽ
			std::cout << "��ǰ˼��ģʽ: " << (ucci.model?"ʱ��":"���") << std::endl;
		}
		else
		{
			std::cout << "δ����״̬��ALT_F1��ALT_F9 �����ӿͻ��ˣ�" << std::endl;
		}
		g_biBitCount = g_biBitCount==24? 32:24;
		std::cout << "��ǰ��Ļ��ͼλ��: " <<  g_biBitCount << " λ" << std::endl;

		break;

	case ALT_F8:	
		//�������ò�����ʼ��Ϊ��ͬ�ľ��棬���͵ģ��ҷ�ִ��ʱ���췽���߻��Ұ��
		//��������ʱ����λ��ʼ����Ϊԭʼ���棬Ҳ����һ�����ಽ��ֻҪȷ��������һ�¼��ɣ�
		link.set("rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w");			

		std::cout<< ((link.make_solution(msg.pt))? "��������OK" : "�����������ɹ���") << std::endl;
		break;

	case ALT_F10:	
		bConnected = bConnected? false:true;
		OPENFILENAME myofn;
		char strFile[MAX_PATH];
		memset(&myofn,0,sizeof(OPENFILENAME));
		memset(strFile,0,sizeof(char)*MAX_PATH);
		myofn.nMaxFile=MAX_PATH;
		myofn.lStructSize=sizeof(OPENFILENAME);
		myofn.Flags=OFN_FILEMUSTEXIST;
		myofn.lpstrFilter="���������ļ�(*.lll)\0*.lll\0\0";	//Ҫѡ����ļ���׺ 
		myofn.lpstrInitialDir = ".";						//Ĭ�ϵ��ļ�·�� 
		myofn.lpstrFile=strFile;							//����ļ��Ļ�����  
		if(GetOpenFileName(&myofn))		//strFile�õ��û���ѡ���ļ���·�����ļ��� 
		{
			PathStripPath(strFile);		//strFile�õ��ļ���
		}

		if(link.load_solution(strFile))									
		{
			std::cout << link.print();
			std::cout << "Load solution ok!!\n" << std::endl;
			std::cout << "please link again!!\n" << std::endl;
		}
		break;

	case ALT_F11:	
		if(bConnected)
		{
			bAutoGo = !bAutoGo;
			std::cout << (bAutoGo? "�Զ����忪!":"�Զ������!") << std::endl;
			if(bAutoGo && link.from.x!=-1)
			{
				link.make_move(link.from, link.to);		//ģ��ͼ������	
				link.from.x=link.from.y=link.to.x=link.to.y=-1;	//����������ɱ�־
				Sleep(500);								//���ٶ�500��������ӳɹ������õİ취�Ǽ���¸����淢���仯�����������þ��Ǹղŵ�bestmove ����á�
				std::cout << "�л����в�δ�ߣ������ꣿ" << std::endl << std::endl;
			}
		}
		else
		{
			std::cout << "δ����״̬��ALT_F1��ALT_F9 �����ӿͻ��ˣ�" << std::endl;
		}
		break;

	case ALT_F12:	
		if(bConnected)
		{
			bGoInfinite = !bGoInfinite;
			if(bGoInfinite)ucci.StopEngine();
			std::cout << (bGoInfinite? "���޷���ģʽ��!":"���޷���ģʽ��!") << std::endl;
		}
		else
		{
			std::cout << "δ����״̬��ALT_F1��ALT_F9 �����ӿͻ��ˣ�" << std::endl;
		}
		break;

	case CTRL_ALT_F1:	
		if(bConnected)
		{
			std::cout << "����״̬���� ALT_F6 �ȶϿ����ӣ�" << std::endl;
		}
		else
		{
			cout << "CTRL_ALT_F1 pressed!\n" << endl;
			cout << "ѭ���޸Ĳ�������q���˳�!" << endl;
			cout << "s save solution!" << endl;
			cout << "l load solution!" << endl;
			char c;
			while((c=getchar())!='q')
			{
				switch(c)
				{
				case 's':
					cout << "�����ѱ���!" << endl;
					break;
				case 'l':
					cout << "����������!" << endl;
					break;
				case 'c':
					g_biBitCount = g_biBitCount==24? 32:24;
					std::cout << "��ǰ��Ļ��ͼλ��: " <<  g_biBitCount << " λ" << std::endl;
					break;
				default:
					cout << "\nѭ���޸Ĳ�������q���˳�!" << endl;
					cout << "s save solution!" << endl;
					cout << "l load solution!" << endl;
					cout << "��ѡ��!" << endl;
					break;
				}
			}
			cout << "���˳��޸Ĳ���!" << endl;
		}
		break;

	case CTRL_ALT_F2:	
		if(bConnected)
		{
			std::cout << "����״̬���� ALT_F6 �ȶϿ����ӣ�" << std::endl;
		}
		else
		{
			std::cout << "��ǰ������С��"<< link.get_width_init() << "�������µ�������С: ";
			std::cin >> width_init;
			std::cout << "��ǰʵʱ�Ӵ�С��"<< link.get_width_real() << "�������µ�ʵʱ�Ӵ�С: ";
			std::cin >> width_real;
			std::cout << "��ǰ�ֱ��ʣ�"<< link.get_threshold() << "�������µķֱ���: ";
			std::cin >> threshold;
			link.set_width_init(width_init);
			link.set_width_real(width_real);
			link.set_threshold (threshold);
			std::cout << "\n����������С��" << link.get_width_init() << std::endl;
			std::cout << "����ʵʱ�Ӵ�С��" << link.get_width_real() << std::endl;
			std::cout << "���·ֱ��ʣ�" << link.get_threshold() << std::endl;

			std::cout << "��ǰ�ȴ�ʱ�䣺"<< timeWaitBM << " �������µ�����ŷ���ʱ�ȴ�ʱ�䣨ȱʡ1000��: ";
			std::cin >> timeWaitBM;
			std::cout << "����ŷ���ʱ�ȴ�ʱ�䣺" << timeWaitBM << std::endl;
		
			int time;
			std::cout << "������˼��ʱ�䣨ȱʡ15000���룩: ";
			std::cin >> time;
			ucci.set_time(time);
			std::cout << "�������µ�˼��ʱ��!" << std::endl;

			std::cout << "������˼����ȣ�ȱʡ8�㣩: ";
			std::cin >> time;
			ucci.set_depth(time);
			std::cout << "�������µ�˼�����!" << std::endl;
			std::cout << "���˳���������!" << std::endl;
		}
		break;

	case CTRL_ALT_F3:	
		if(bConnected)
		{
			bBH = !bBH;
			std::cout << (bBH? "���ӷ�����!":"���ӷ�����!") << std::endl;
		}
		else
		{
			std::cout << "�ͻ���δ���ӣ��� ALT_F1 �� ALT_F9 �����ӣ�" << std::endl;
		}
		break;

	case CTRL_ALT_F4:	
		if(bConnected)
		{
			ucci.StopEngine();
			std::cout << "������ֹͣ˼����������������" << std::endl;
		}
		else
		{
			std::cout << "����δ���ӣ����洦�ڿ���״̬��ͣʲôͣ :) " << std::endl;
		}
		//link.sideToMove=1-link.sideToMove;
		//std::cout << "\n����������Ȩ�������ֵ���"<< (link.sideToMove==link.local_color()?"�ҷ���":"�Է���")<< std::endl;
		break;

	default:
		break;	
	}
}
