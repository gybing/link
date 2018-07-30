#include <iostream>

#include "thread.h"
#include "linker.h"
#include "lock.h"
#include "ucci.h"
#include "hotkey.h"

extern HANDLE	g_bmEvent;	
int timeWaitBM	=	500;		//

//�����������״̬�߳�
//��ȡ����ʾbestmove��pv������
//����bestmove�����Զ�����ģ�����������POINT
//����׼�����źţ����������߳�
DWORD CALLBACK wait_bestmove_threadproc(PVOID pvoid)
{
	while(true)
	{
		CLockRegion _lock(&lock);	//_lock�����������ù��캯���������������ٽ���
		while(ucci.ReceiveUCCI())
		{	
			string lines(ucci.szLineStr);
			//������Լ�⵽���淵�صĲ�ͬ����ֱ���
			if( char* bestmove=strstr(ucci.szLineStr,"nobestmove") )		//���ּ�ⷽ���������ȼ��nobestmove�Ų������
			{
				//CLockRegion _lock(&lock);	//_lock�����������ù��캯���������������ٽ���
				link.from.x = link.to.x = -1;
				link.from.y = link.to.y = -1;
                std::cout << "nobestmove\n" << std::endl;
			}
			else if( char* bestmove=strstr(ucci.szLineStr,"bestmove") )		//bestmove
			{
				string str(ucci.szLineStr);

				Square frm = square_from_string(str.substr(0+9, 2));
				Square tto = square_from_string(str.substr(2+9, 4));
				Move bstmv = Move( (frm << 8) | tto );

				string title = link.GetStepName(bstmv);
				if(link.local_color()==BLACK)	//�·�Ϊ�ڷ��������岽����任����Ϊ֮ǰ��ת�������淵���߲���������ʽ���õ����ĵı�ʾ�������Ǻ���ת֮������̡�
				{  
					link.ChangeSide();
					title = link.GetStepName(bstmv);
					link.ChangeSide();
				}

		        //CLockRegion _lock(&lock);	//_lock�����������ù��캯���������������ٽ���
				//h7e7   (7,7)-(4,7)   �ڶ�ƽ��
				link.from.x = bestmove[9]-'a';
				link.from.y = 9 - (bestmove[10]-'0');
				link.to.x =   bestmove[11]-'a';
				link.to.y =   9 - (bestmove[12]-'0');
				if(link.local_color()==BLACK)			//����·�Ϊ�ڷ����򷵻ص��岽Ҫ�任һ��
				{
					link.from.x = 8 -link.from.x ;
					link.from.y = 9 -link.from.y ;
					link.to.x = 8 -link.to.x ;
					link.to.y = 9 -link.to.y ;
				}
				std::cout << "����ŷ��ѷ��أ� " << title << std::endl;
				SetEvent(g_bmEvent);					//����bestmove׼�����ź�״̬
			}
			else if(char* pvstr = strstr(ucci.szLineStr,"pv"))				//���� pvstr
			{
				string ucipv(pvstr);
				std::vector<Move> pv;	//pv.push_back(MOVE_NONE);

				//CLockRegion _lock(&lock);	//_lock�����������ù��캯���������������ٽ���

				if( ucipv.size()>= 7 )	//����һ���߷�  "pv b2e2"  "pv b2e2 b7e7 a0a2";
				{
					CMatLinker curr;
					
					curr.set(link.fen());
					if(link.local_color()==BLACK)	//�·�Ϊ�ڷ��������岽����任����Ϊ֮ǰ��ת�������淵���߲���������ʽ���õ����ĵı�ʾ�������Ǻ���ת֮������̡�
					{  
						curr.ChangeSide();
					}

					cout << "curr pv: " ;
					for(unsigned int i =0; i< (ucipv.size()-2)/5; i++)
					{
						string uci_move = ucipv.substr(0+3+5*i, 4);
						Square frm = square_from_string(uci_move.substr(0, 2));
						Square tto = square_from_string(uci_move.substr(2, 2));
						Move bstmv = Move( (frm << 8) | tto );
						cout << curr.GetStepName(bstmv) << " ";
						curr.do_move(bstmv);
					}
					cout << endl; 
				}
			}
			else if(char* pvstr = strstr(ucci.szLineStr,"depth"))
			{
				string ucipv(pvstr);
				//cout << ucipv << endl; 
			}
			else if(char* pvstr = strstr(ucci.szLineStr,"time"))
			{
				string ucipv(pvstr);
				//cout << ucipv << endl; 
			}
		}
        //�������˳���_lock���������������뿪�ٽ���
	}
	return 0;
}

//ѭ�������߳�
//�����̵߳Ķ�ʱ�����ڸ��¾���
//���̺߳����ж��Ƿ���淢���仯������˫�����Ӷ���
//���ֵ��ҷ���������������㣬�������ȴ���һ���̼߳�ⷵ�����������bestmove
//���ֵ�Զ�����ӣ���ȴ����ߺ�̨˼���Է��з�
DWORD WINAPI ThreadProcLoopMove(LPVOID lparam)
{
	static CMatLinker old;

	while(true)
	{
		if(!bConnected)
		{
			continue;
		}
		while(bConnected && WAIT_OBJECT_0 != WaitForSingleObject(g_bmEvent, timeWaitBM))		//�����ȴ����󷵻� bestmove���ȴ�ʱ��Ӧ����Ϊ�ɱ���������ʵ�ֶ�ʱ����ÿ��һ��ʱ��������档
		{
			CLockRegion _lock(&lock);				//_lock�����������ù��캯���������������ٽ���
			//��������
			link.update_fen();

			//�ǳ�ʼ���棬����ǰ������仯���ȷ����ǰ���ӷ�
			if(link.fenold!="")						
			{
				old.set((char*)link.fenold.c_str());
				int diffs = link.board2diffs(old.m_Board);
				if(diffs==0)	//����δ�����仯������ѭ��
				{
					continue;	
				}
				if(diffs==2)	//���ҽ���һ�������ӣ���������Ȩ
				{
					link.sideToMove = 1- link.sideToMove ;	
				}
			}
			link.fenold = link.fen();			//���¾ɴ�
			std::cout << link.print() << std::endl;
			std::cout << "\n�����ֵ���"<< (link.sideToMove==link.local_color()?"�ҷ���":"�Է���")<< std::endl;
			
			
			//�������������������������ã�
			string fentoengine = link.fen();			//�������͸���������fen��
			if(bBH)	start_bh(link.fen(), link.sideToMove ==  link.local_color());				//�����ⲿ�������������

			//������������������
			if( link.sideToMove ==  link.local_color() )	//�ֵ��ҷ���,ע�͵���ɷ���˫��
			{
				ucci.StopEngine();
				if(link.local_color()==BLACK)				//����·�Ϊ�ڷ����޶�һ��Fen������ucci�����ȱ��Լ�������º��ϣ�
				{
					link.ChangeSide();
					fentoengine = link.fen();	
					link.ChangeSide();
				}
				if(bAutoGo)
				{
					ucci.RunEngine((char*)fentoengine.c_str());
					std::cout << "�Զ����Ӵ򿪣� ����������������ȴ����ؽ����ALT_F7���л���Ȼ�ʱ�����ģʽ��......" << std::endl;
				}
				else if(bGoInfinite)
				{
					static const int BUFFERSIZE=1024;
					static char buffer[BUFFERSIZE];
					char *lpLineChar = buffer;
					lpLineChar += sprintf_s(lpLineChar, BUFFERSIZE, "position fen %s ", (char*)fentoengine.c_str());
					ucci.UCCI2Engine(buffer);					//���;�����Ϣ
					ucci.UCCI2Engine("go infinite");            //�������޷���ģʽ
					std::cout << "�Զ����ӹرգ� �����������޷���ģʽ����ALT_F12Ҳ�ɿ�������˼��ģʽ��" << std::endl;
				}
			}
		}

		//���ˣ�bestmove���ء�����һ���߳������źű�־
		ResetEvent(g_bmEvent);										//ϵͳ���Զ�������
		std::cout << "bestmove �ѷ��أ� ALT_F11�л��Զ����ӣ�" << std::endl;
		if( bAutoGo && link.sideToMove == link.local_color())		//�Զ����ӿ��ش򿪣������ֵ��·�����
		{
			link.make_move(link.from, link.to);						//ģ��ͼ������	
			link.from.x=link.from.y=link.to.x=link.to.y=-1;			//����������ɱ�־
			Sleep(500);												//���ٶ�500��������ӳɹ������õİ취�Ǽ���¸����淢���仯�����������þ��Ǹղŵ�bestmove ����á�
			std::cout << "500�����Զ���������ɣ�" << std::endl << std::endl;
		}
		//�������˳���_lock���������������뿪�ٽ���
	}
	return 0;
}