#include "pipe.h"
#include "prog.h"

#ifndef UCCI_H
#define UCCI_H


const int MAX_IRREV_POS = 33;   // ������������������������ֳ��������ᳬ��32��
const int MAX_IRREV_MOVE = 160; // ��������������ŷ������������ŷ�����������80�غ�����
const int MAX_BAN_MOVE = 128;   // ���Ľ�ֹ�ŷ���
const int MAX_INFO = 16;        // �汾��Ϣ���������
const int MAX_OPTION = 16;      // ѡ�����õ��������
const int MAX_LEVEL = 16;       // �Ѷȵ���߼�����

/* ���³���������������˼��״̬����̨˼���Ĵ�����һ���ѵ㣺
 * (1) �����ú�̨˼��ʱ������״̬��"IDLE_NONE"������˼��״̬��"BUSY_THINK"����ʾ˼��״̬��"BUSY_HINTS"��
 * (2) ���ú�̨˼��ʱ������˼�������󣬾ͽ����̨˼��״̬(BUSY_PONDER)����"mvPonder"���ǲ²��ŷ���
 * (3) "BUSY_PONDER"״̬�£�������ָ������ŷ�û���ú�̨˼�����У����̨˼���жϣ�
 * (4) "BUSY_PONDER"״̬�£�������ָ������ŷ��ú�̨˼������(��"mvPonder"һ��)���ͽ����̨˼������״̬(BUSY_PONDERHIT)��
 * (5) "BUSY_PONDER"״̬�£������̨˼������(�ڶ��ָ����ŷ�֮ǰ)��������̨˼�����״̬(IDLE_PONDER_FINISHED)����"mvPonderFinished"�򱣴��̨˼���Ľ����
 * (6) "BUSY_PONDERHIT"״̬�£�����յ�˼��ָ���ת������˼��״̬(BUSY_THINK)��
 * (7) "BUSY_PONDERHIT"״̬�£������̨˼������(�ڶ��ָ����ŷ�֮ǰ)����ת���̨˼����ɲ�������״̬(IDLE_PONDERHIT_FINISHED)����"mvPonderFinished"�򱣴��̨˼���Ľ����
 * (8) "IDLE_PONDER_FINISHED"״̬�£�������ָ������ŷ�û���ú�̨˼�����У���������¿�ʼ˼����
 * (9) "IDLE_PONDER_FINISHED"״̬�£�������ָ������ŷ��ú�̨˼�����У���ת���̨˼����ɲ�������״̬(IDLE_PONDERHIT_FINISHED)��
 * (10) "IDLE_PONDERHIT_FINISHED"״̬�£�����յ�˼��ָ�����������"mvPonderFinished"�ŷ���
 */
const int IDLE_NONE = 0;
const int IDLE_PONDER_FINISHED = 1;
const int IDLE_PONDERHIT_FINISHED = 2;
const int BUSY_WAIT = 3;
const int BUSY_THINK = 4;
const int BUSY_HINTS = 5;
const int BUSY_PONDER = 6;
const int BUSY_PONDERHIT = 7;

class   CUcci
{
 public:
      PipeStruct pipeEngine;                        // UCCI����ܵ�������"pipe.cpp"

      // ������״̬ѡ��
      bool bDebug, bUcciOkay, bBgThink;             // �Ƿ����ģʽ��UCCI�����Ƿ���������̨˼���Ƿ�����
      int nLevel, nStatus;                          // �����״̬
      char * mvPonder;
      char * mvPonderFinished;        // ��̨˼���Ĳ²��ŷ��ͺ�̨˼����ɵ��ŷ�
      // UCCI����������Ϣ
      char szUCCICommand[1024];                               // UCCI ���
      char szUCCIReplyLine[1024];                               // UCCI ���ش�
      char szEngineName[1024], szEngineFile[1024];            // UCCI�������ƺ�UCCI��������ļ���ȫ·��
      char szInfoStrings[16][256], szOptionStrings[16][1024]; // �汾��Ϣ��ѡ������
      char szLevelStrings[16][256], szThinkModes[16][256];    // �Ѷȼ���͸����Ѷȼ����µ�˼��ģʽ

      char szLineStr[LINE_INPUT_MAX_CHAR];

      bool LoadEngine(void);      // ���أգãã�����
      bool UnLoadEngine(void);      // ȥ�أգãã�����
      void UCCI2Engine( const char * eCommand );   // ��UCCI���淢�͸���ָ��
      bool ReceiveUCCI(void);      // UCCI������Ϣ�Ľ��չ���

      char * RunEngine(char * FenStr);   // ��UCCI���淢��˼��ָ��
      void StopEngine(void);

      CUcci();
      ~CUcci();

} ;



#endif

