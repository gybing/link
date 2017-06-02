
/********************************************************************************
ģ����         : ��չ��
�ļ���         : board.cpp
����ļ�       : board.h
�ļ�ʵ�ֹ���   : 
����           :
�汾           : 1.10
--------------------------------------------------------------------------------
��ע           :
--------------------------------------------------------------------------------
�޸ļ�¼ :
��  ��          �汾            �޸���          �޸�����
2003/08/27      1.0                             ����
2003/12/29      1.1                             ����������ʽ��

================================================================================
* ��Ȩ����(c) 2014, 2015, , ��������Ȩ��
********************************************************************************/

#include "board.h"

#pragma warning( disable:4996 )				//���κ�����ȫ�Ծ���
#pragma warning( disable:4101 )				//����δʹ�ñ�������

CNewQiJu :: CNewQiJu(): m_PlayerTop(m_PlayerUcci) , m_PlayerBottom(m_PlayerUcci) //��ʼ���б�ʽ��ʼ����Ա����
{
      memcpy(m_ChessBoard,InitChessBoard,90);
      m_bBottomGo=true ;
      m_WhichPlayer =m_bBottomGo?&m_PlayerBottom:&m_PlayerTop;

      m_PlayerBottom.m_iChessMoveFrom =ENGINE_PLAYER ;
//      m_PlayerBottom.m_bProgBottom = true;

      m_PlayerTop.m_iChessMoveFrom = BMP_PLAYER ;
//      m_PlayerTop.m_bProgBottom=false;

      m_bConnected = false;
      m_bGameOver=false ;
}

CNewQiJu ::~CNewQiJu()
{
}

void CNewQiJu :: WCHAR2CHESSMOVE(WCHAR wsChessMove[5], CHESSMOVE &cmChessMove )
{
    WCHAR * qian = L"ǰ" ;
    WCHAR * hou = L"��" ;
    WCHAR * jing = L"��" ;
    WCHAR * tui  =  L"��" ;
    WCHAR * ping = L"ƽ" ;
    //WCHAR   qizi[]=L"�ս�������ʿ����˧�����������" ;
    WCHAR   qizi[]=L"�ս�������ʿ����˧������ʿ���" ;
    WCHAR   weiziblack[]=L"123456789" ;
    WCHAR   weiziblackq[]=L"������������������" ;
    WCHAR   weizired[]=L"һ�����������߰˾�" ;
    bool    Red;
    WCHAR   shi[10]=L"��ʿ";

    int     ChessID = NOCHESS ;
    int     x1,y1,x2,y2 ;
//   always assume black is above;
//    if(!m_PlayerBottom.m_bQiShouRed)
//    {
//         ChangeSide(m_ChessBoard);
//    }
//
   WCHAR * pweizi ;
   int 	   i,j;
   WCHAR *wchar =L"a";
   for(int oo=0;oo<4;oo++)
   {
      for(int ll=0;ll<9;ll++)
      {
          if(wsChessMove[oo]==weiziblackq[ll] )
          {
             wsChessMove[oo]=weiziblack[ll];
          }
          if( wsChessMove[oo]==shi[0])
          {
             wsChessMove[oo]=shi[1];
          }
      }
   }
    if(m_bBottomGo)
    {
        Red = m_PlayerBottom.m_bQiShouRed;
    }
    else
    {
        Red = m_PlayerTop.m_bQiShouRed;
    }
    if(Red)
    {
         pweizi=weizired;
         i=8;
    }
    else
    {
        pweizi=weiziblack;
         i=0;
    }
    if((wsChessMove[0]==qian[0])||(wsChessMove[0]==hou[0]))
    {
        wchar[0]=wsChessMove[1];
    }
    else
    {
        wchar[0]=wsChessMove[0];
    }

    for( i; i<15; i++ )
     {
         if( wchar[0]  == qizi[i] )
         {
               ChessID=i;  break;

         }
     }

     if(wsChessMove[0]==qian[0] && m_bBottomGo || wsChessMove[0]==hou[0] && !m_bBottomGo )
        {
              for( int ii=0;ii<9;ii++)
              for( int jj=0;jj<10;jj++)
             {
                  if( m_ChessBoard[jj][ii] == ChessID)
                  {
                       x1=ii;       y1=jj;     break;
                  }
             }
         }
     else if(wsChessMove[0]==qian[0] && !m_bBottomGo  || wsChessMove[0]==hou[0]&&m_bBottomGo )
        {
              for( int ii=0;ii<9;ii++)
              for( int jj=9;jj>=0;jj--)
             {
                  if( m_ChessBoard[jj][ii] == ChessID)
                  {
                       x1=ii;       y1=jj;  break;
                  }
             }
         }

   else
     {
         for( j=0;j<9;j++)
         {
             if( wsChessMove[1] == pweizi[j] )
             {
                   if(m_bBottomGo)   x1=8-j;
                   else          x1=j;
                   break;
             }
         }

         for( int jj=0;jj<10;jj++)
         {
              if( m_ChessBoard[jj][x1] == ChessID )
              {
                        y1=jj;     break;
              }
         }
     }

////////////////////////////////////////////////////////////////////////
     if(wsChessMove[2] == ping[0])
     {
           for( j=0;j<15;j++)
           {
               if( wsChessMove[3] == pweizi[j] )
               {
                     if(m_bBottomGo)  x2=8-j;
                     else         x2=j;
                     y2=y1;
                     break;
               }
           }
     }

   if(wsChessMove[2] == jing[0])
     {
           for( j=0;j<9;j++)
           {
               if( wsChessMove[3] == pweizi[j] )
               {
                     if(ChessID==B_HORSE ||ChessID==R_HORSE)
                     {
                              if(m_bBottomGo)x2=8-j; else x2=j;
                              if(abs(x1-x2)==1)
                              {
                                 if(m_bBottomGo)  y2=y1-2;  else y2=y1+2;
                              }
                              else
                              {
                                 if(m_bBottomGo)  y2=y1-1;  else y2=y1+1;
                              }
                              break;
                     }
                     else if(ChessID==B_BISHOP ||ChessID==R_BISHOP)
                     {
                                if(m_bBottomGo)  x2=8-j;     else x2=j;
                                if(m_bBottomGo)
                                {
                                        y2=y1-1;
                                        if(y2<7)
                                        {
                                                y1+=2;
                                                y2=y1-1;
                                        }
                                }
                                else
                                {
                                        y2=y1+1;
                                        if(y2>2)
                                        {
                                                y1-=2;
                                                y2=y1+1;
                                        }
                                }
                                break;
                     }
                     else if(ChessID==B_ELEPHANT ||ChessID==R_ELEPHANT)
                     {
                                if(m_bBottomGo)x2=8-j; else x2=j;
                                if(m_bBottomGo)
                                {
                                        y2=y1-2;
                                        if(y2<5)
                                        {
                                                y1+=4;
                                                y2=y1-2;
                                        }
                                }
                                else
                                {
                                        y2=y1+2;
                                        if(y2>4)
                                        {
                                                y1-=4;
                                                y2=y1+2;
                                        }
                                }
                                break;
                     }
                     else
                     {
                                 x2=x1;
                                 if(m_bBottomGo) y2=y1-j-1;     else y2=y1+j+1;
                                 break;

                     }
               }
           }
     }

    if(wsChessMove[2] == tui[0])
     {
           for( j=0;j<9;j++)
           {
               if( wsChessMove[3] == pweizi[j] )
               {
                     if(ChessID==B_HORSE ||ChessID==R_HORSE)
                     {
                              if(m_bBottomGo)x2=8-j; else x2=j;
                              if(abs(x1-x2)==1)
                              {
                                    if(m_bBottomGo)  y2=y1+2;   else y2=y1-2;
                              }
                              else
                              {
                                    if(m_bBottomGo)  y2=y1+1;   else y2=y1-1;
                              }
                              break;
                     }
                     else if(ChessID==B_BISHOP ||ChessID==R_BISHOP) //error
                     {
                                if(m_bBottomGo)x2=8-j; else x2=j;
                                if(m_bBottomGo)
                                {
                                        y2=y1+1;
                                        if(y2>9)
                                        {
                                                y1-=2;
                                                y2=y1+1;
                                        }
                                }
                                else
                                {
                                        y2=y1-1;
                                        if(y2<0)
                                        {
                                                y1+=2;
                                                y2=y1-1;
                                        }
                                }
                                break;
                     }
                     else if(ChessID==B_ELEPHANT ||ChessID==R_ELEPHANT) //error
                     {
                                if(m_bBottomGo)x2=8-j; else x2=j;
                                if(m_bBottomGo)
                                {
                                        y2=y1+2;
                                        if(y2>9)
                                        {
                                                y1-=4;
                                                y2=y1+2;
                                        }
                                }
                                else
                                {
                                        y2=y1-2;
                                        if(y2<0)
                                        {
                                                y1+=4;
                                                y2=y1-2;
                                        }
                                }
                                break;
                     }
                     else
                     {
                                 x2=x1;
                                 if(m_bBottomGo)  y2=y1+j+1; else y2=y1-j-1;
                                 break;
                     }
               }
           }
     }

    cmChessMove.ChessID=ChessID;
    cmChessMove.From.x=x1;
    cmChessMove.From.y=y1;
    cmChessMove.To.x=x2;
    cmChessMove.To.y=y2;
}

void CNewQiJu :: ChangeSide(BYTE position[10][9])
{
    BYTE temp[10][9];
    int i,j ;
    memcpy(temp,position,90);
    for(i=0;i<10;i++)
    for(j=0;j<9;j++)
    {
        if(IsBlack(temp[i][j]))
        {
            temp[i][j] += 7 ;
        }
        else if(IsRed(temp[i][j]))
        {
            temp[i][j] -= 7 ;
        }
    }
    memcpy(position,temp,90);
}

int CNewQiJu :: IsGameOver(BYTE position[10][9])
{
    BOOL RedLive=FALSE,BlackLive=FALSE ;
    int i,j ;
    for(i=7;i<10;i++)
    for(j=3;j<6;j++)
    {
        if(position[i][j]==B_KING)
        BlackLive=TRUE ;
        if(position[i][j]==R_KING)
        RedLive=TRUE ;
    }

    for(i=0;i<3;i++)
    for(j=3;j<6;j++)
    {
        if(position[i][j]==B_KING)
        BlackLive=TRUE ;
        if(position[i][j]==R_KING)
        RedLive=TRUE ;
    }
    if(RedLive&&BlackLive)
    {
        return FALSE ;
    }
    else
    {
        m_bGameOver=true ;
        return TRUE ;
    }
}

void CNewQiJu ::Start()
{
      unsigned char killed ;

      //������ʼ����
      m_PlayerTop.GetBMPBoard();
      m_PlayerTop.BMP2Position();
      memcpy(m_ChessBoard, m_PlayerTop.m_position,90);

      while( m_bConnected )       //���Ӻ�ѭ��  //���߳�����ʱ��������VCL������.....
      {
          //һ��˭�ߣ�
          m_WhichPlayer = m_bBottomGo? &m_PlayerBottom:&m_PlayerTop;

          //������ô�ߣ�
          bool  moveOK=false;
          //TDateTime startDateTime=Now(), currentDateTime;
          do
          {     //�����ͼ�οͻ��ˣ��˴�����ɨ��Ƶ��
                //currentDateTime=Now();
                //if((currentDateTime.Val-startDateTime.Val)*360*24 > 1) //1����
                if(!m_bConnected)    //���ӶϿ�
                {
                     // Application->MessageBox("GetQiShouMove ��ʱ��\n�˳�����...","���",MB_OK);
                     // m_bConnected=false;
                     break;
                }
                m_cmQiJuMove = m_WhichPlayer->GetMove(m_ChessBoard) ;
                moveOK = (m_cmQiJuMove.From.y!=-1 && m_cmQiJuMove.From.x!=-1);
          }while(!moveOK);

          if(moveOK)
          {
                //�����߰�
                killed = MakeMove(&m_cmQiJuMove);//������Cnewqijuʵ�����ݣ�����������Ȩ���������ͻ�������
                switch(m_WhichPlayer->m_iChessMoveFrom) //������ɶ���Ĵ���
                {
                  case ENGINE_PLAYER:
                       if(strcmp(m_WhichPlayer->m_ucci.szLineStr, "nobestmove")==0)//����������
                       {
                           m_bConnected=false;    //�Ͽ�����
                           break;
                       }
                       break;

                  case BMP_PLAYER:  //�ҷ��ǿͻ��ˣ������岽�������ҷ��ͻ�������һ��      //�����֧����
                       if(!m_WhichPlayer->BMPGo(m_cmQiJuMove ,!m_bBottomGo) )
                       {
                         //ShowMessage("�ͻ������Ӵ���");
                         UnMakeMove(  &m_cmQiJuMove,killed);
                       }
                       Sleep(1000);
                       break;

                  case VOICE_PLAYER:
                       break;

					default:
					  break;
                }
          }
     }
}

BYTE CNewQiJu ::MakeMove(CHESSMOVE* move)
{
	BYTE nChessID;
	nChessID = m_ChessBoard[move->To.y][move->To.x];
	m_ChessBoard[move->To.y][move->To.x] = m_ChessBoard[move->From.y][move->From.x];
	m_ChessBoard[move->From.y][move->From.x] = NOCHESS;
        m_bBottomGo=!m_bBottomGo;
        m_WhichPlayer=m_bBottomGo?&m_PlayerBottom:&m_PlayerTop;
        m_iTurn++;
	return nChessID;
}

void CNewQiJu::UnMakeMove(CHESSMOVE* move,BYTE nChessID)
{
	m_ChessBoard[move->From.y][move->From.x] = m_ChessBoard[move->To.y][move->To.x];
	m_ChessBoard[move->To.y][move->To.x] = nChessID;
        m_bBottomGo=!m_bBottomGo;
        m_WhichPlayer=m_bBottomGo?&m_PlayerBottom:&m_PlayerTop;
        m_iTurn--;
}

BOOL CNewQiJu::IsValidMove(BYTE position[10][9],CHESSMOVE mv)
{
        // only according to position: black on top and red below!

	int i, j;
	int nMoveChessID, nTargetID;
        int nFromX=mv.From.x;
        int nFromY=mv.From.y;
        int nToX=mv.To.x;
        int nToY=mv.To.y;

	if (nFromY ==  nToY && nFromX == nToX)
		return FALSE;//Ŀ����Դ��ͬ
	
	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];
	
	if (IsSameSide(nMoveChessID, nTargetID))
		return FALSE;//���ܳ��Լ�����
	
	switch(nMoveChessID)
	{
	case B_KING:     
		if (nTargetID == R_KING)//�Ͻ�����?
		{
			if (nFromX != nToX)
				return FALSE;
			for (i = nFromY + 1; i < nToY; i++)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;
		}
		else
		{
			if (nToY > 2 || nToX > 5 || nToX < 3)
				return FALSE;//Ŀ����ھŹ�֮��
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;//��˧ֻ��һ��ֱ��:
		}
		break;
	case R_BISHOP:   
		
		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��
		
		break;
		
	case B_BISHOP:   //��ʿ

		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��
		
		break;
		
	case R_ELEPHANT://����
		
		if(nToY < 5)
			return FALSE;//�಻�ܹ���
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//��������

		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��
		
		break;
		
	case B_ELEPHANT://���� 
		
		if(nToY > 4)
			return FALSE;//�಻�ܹ���
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//��������

		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��
		
		break;
		
	case B_PAWN:     //�ڱ�
		
		if(nToY < nFromY)
			return FALSE;//������ͷ
		
		if( nFromY < 5 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��
		
		if(nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:
		
		break;
		
	case R_PAWN:    //���
		
		if(nToY > nFromY)
			return FALSE;//������ͷ
		
		if( nFromY > 4 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��
		
		if(nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:

		break;
		
	case R_KING:     
		if (nTargetID == B_KING)//�Ͻ�����?
		{
			if (nFromX != nToX)
				return FALSE;//����������ͬһ��
			for (i = nFromY - 1; i > nToY; i--)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;//�м��б����
		}
		else
		{
			if (nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;//Ŀ����ھŹ�֮��
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1) 
				return FALSE;//��˧ֻ��һ��ֱ��:
		}
		break;
		
	case B_CAR:
	case R_CAR:      
		
		if(nFromY != nToY && nFromX != nToX)
			return FALSE;	//����ֱ��:
		
		if(nFromY == nToY)
		{
			if(nFromX < nToX)
			{
				for(i = nFromX + 1; i < nToX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(i = nToX + 1; i < nFromX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if(nFromY < nToY)
			{
				for(j = nFromY + 1; j < nToY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(j= nToY + 1; j < nFromY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}
		
		break;
		
	case B_HORSE:    
	case R_HORSE:
		
		if(!((abs(nToX-nFromX)==1 && abs(nToY-nFromY)==2)
			||(abs(nToX-nFromX)==2&&abs(nToY-nFromY)==1)))
			return FALSE;//��������
		
		if	(nToX-nFromX==2)
		{
			i=nFromX+1;
			j=nFromY;
		}
		else if	(nFromX-nToX==2)
		{
			i=nFromX-1;
			j=nFromY;
		}
		else if	(nToY-nFromY==2)
		{
			i=nFromX;
			j=nFromY+1;
		}
		else if	(nFromY-nToY==2)
		{
			i=nFromX;
			j=nFromY-1;
		}

		if(position[j][i] != NOCHESS)
			return FALSE;//������
		
		break;
	
	case B_CANON:    
	case R_CANON:    
		
		if(nFromY!=nToY && nFromX!=nToX)
			return FALSE;	//����ֱ��:
		
		//�ڲ�����ʱ������·���в���������
		
		if(position[nToY][nToX] == NOCHESS)
		{
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i = nFromX + 1; i < nToX; i++)
						if(position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(i = nToX + 1; i < nFromX; i++)
						if(position[nFromY][i]!=NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if(nFromY < nToY)
				{
					for(j = nFromY + 1; j < nToY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(j = nToY + 1; j < nFromY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//�ڳ���ʱ
		else	
		{
			int count=0;
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i=nFromX+1;i<nToX;i++)
						if(position[nFromY][i]!=NOCHESS)
							count++;
						if(count != 1)
							return FALSE;
				}
				else
				{
					for(i=nToX+1;i<nFromX;i++)
						if(position[nFromY][i] != NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
			}
			else
			{
				if(nFromY<nToY)
				{
					for(j=nFromY+1;j<nToY;j++)
						if(position[j][nFromX]!=NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
				else
				{
					for(j=nToY+1;j<nFromY;j++)
						if(position[j][nFromX] != NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
			}
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}