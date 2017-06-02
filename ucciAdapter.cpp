#pragma hdrstop

#include <stdio.h>
#include "ucciAdapter.h"

CUcci :: CUcci()
{
      LoadEngine();
      //strcpy(szEngineName,"myucci");
	  strcpy_s(szEngineName, 1024, "myucci");
}
CUcci :: ~CUcci()
{
      UnLoadEngine();
}
bool CUcci::LoadEngine(void)
{
      bUcciOkay = false;
      pipeEngine.Open("jqxq.exe");
      pipeEngine.LineOutput("ucci");
      pipeEngine.LineOutput("isready");
      while(ReceiveUCCI()) ;
      return bUcciOkay;
}

bool CUcci::UnLoadEngine(void)
{
      pipeEngine.LineOutput("quit");
      while(ReceiveUCCI()) ;
      pipeEngine.Close();
      return !bUcciOkay;
}

void CUcci::UCCI2Engine( const char * eCommand )
{
       pipeEngine.LineOutput( eCommand );
}

// UCCI������Ϣ�Ľ��չ���
bool CUcci::ReceiveUCCI(void)
{
      if (!pipeEngine.LineInput(szLineStr))
      {
          return FALSE;
      }
      if (strncmp(szLineStr, "bestmove ", 9) == 0)
      {
            nStatus =  IDLE_NONE ;

            strcpy_s(mvPonder,  szLineStr + 9);
            mvPonder[4] = 0;

            strcpy_s(mvPonderFinished,  szLineStr + 9);
            if ( strncmp(szLineStr + 13, " ponder ", 8) == 0)
            {
                nStatus =  IDLE_PONDER_FINISHED ;
                strcpy_s(mvPonder,  szLineStr +21);
            }
      }
      else if (strcmp(szLineStr, "nobestmove") == 0)
      {
            nStatus =  IDLE_NONE ;
      }
      else if (strcmp(szLineStr, "bye") == 0)
      {
            bUcciOkay = FALSE;
      }
      else if (strcmp(szLineStr, "ucciok") == 0)
      {
           nStatus =  IDLE_NONE ;
           bUcciOkay = TRUE;
      }
      return TRUE;
}


char* CUcci::RunEngine(char * FenStr)
{
	  static const int BUFFERSIZE=1024;
      char *lpLineChar;
      static char buffer[BUFFERSIZE];

      // 0.����æ��־
      nStatus =  BUSY_THINK ;

      // 1. ���;�����Ϣ��������ʼ�Ĳ�����FEN����һϵ�к����ŷ�(��ͬ��̨˼���Ĳ²��ŷ�)��
      lpLineChar = buffer;
      //lpLineChar += sprintf(lpLineChar, "position fen %s ", FenStr);
      lpLineChar += sprintf_s(lpLineChar,BUFFERSIZE,"position fen %s ", FenStr);
      pipeEngine.LineOutput( buffer );

      // 2. ����˼��ָ�
      // sprintf(FenStr, nStatus == BUSY_PONDER ? "go ponder %s" : "go %s", FenStr);
       pipeEngine.LineOutput(" go depth 3");
       return buffer;
}

// ��ֹUCCI�����˼��
void CUcci::StopEngine(void)
{
      pipeEngine.LineOutput("stop");
}


