/********************************************************************************
ģ����         : ��չ��
�ļ���         : prog.cpp
����ļ�       : prog.h
�ļ�ʵ�ֹ���   : �๦����չ
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fstream>
#include <assert.h>
#include <iomanip>

#include <windows.h>

#include "toolsCV.h"
#include "board.h"
#include "bmpPlayer.h"
//#include "screenCapture.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/legacy/legacy.hpp"

#pragma warning( disable:4996 )				//���κ�����ȫ�Ծ���
#pragma warning( disable:4101 )				//����δʹ�ñ�������

using namespace cv;
using namespace std;

	string nameID[15]={"nochess", "BKing", "BRook", "BKnight", "BCannon", "BBishop", "BAdvisor", "BPawn", "RKing", "RRook", "RKnight", "RCannon", "RBishop", "RAdvisor", "RPawn"};

	//CMatPlayer::CMatPlayer( )
//{
//    strcpy_s(m_ptcName, 50, "link.lll"); //��������,�����ͼ�οͻ�����Ϊ���ڱ���ؼ���
//    m_LRookInBoard.x = 52;
//    m_LRookInBoard.y = 62;
//    m_LRookInProg.x = 66;
//    m_LRookInProg.y = 120;
//    m_width=40;							 //�ֱ��� ���������ӿ� = ��
//    m_iDD=0;							 //���,���ƶȡ�
//    m_pBoardBitmapBits = NULL;			 //ָ��λͼ���ݿ飨��ʼ�������ݣ�
//    m_pBMPQiziBits = NULL;
//    m_pCurBMPBoardBits = NULL;
//    if(GetFromFile(m_ptcName))
//    {
//        FindWinBoard(); //װ�뷽������Ҫ��ԭ���ϲ�����Ϣ
//    }
//}

CMatPlayer::CMatPlayer(CUcci& ucci, string config, BYTE width, double f):m_ucci(ucci)
{
	strcpy_s(m_ptcName, 50, config.c_str()); //��������,�����ͼ�οͻ�����Ϊ���ڱ���ؼ���
    m_LRookInBoard.x = 52;
    m_LRookInBoard.y = 62;
    m_LRookInProg.x = 66;
    m_LRookInProg.y = 120;
    m_width=width;						 //�ֱ��� ���������ӿ� = ��
    m_iDD=0;							 //���,���ƶȡ�
    m_pBoardBitmapBits = NULL;			 //ָ��λͼ���ݿ飨��ʼ�������ݣ�
    m_pBMPQiziBits = NULL;
    m_pCurBMPBoardBits = NULL;
	m_threshold = f;

    if(GetFromFile(m_ptcName))
    {
        FindWinBoard(); //װ�뷽������Ҫ��ԭ���ϲ�����Ϣ
    }
}

CMatPlayer::~CMatPlayer()
{
    if( m_pBoardBitmapBits != NULL  )
    {
          delete   m_pBoardBitmapBits ;
          m_pBoardBitmapBits =NULL;
    }
    if( m_pCurBMPBoardBits != NULL  )
    {
          delete   m_pCurBMPBoardBits ;
          m_pCurBMPBoardBits =NULL;
    }
    if( m_pBMPQiziBits != NULL  )
    {
          delete   m_pBMPQiziBits ;
          m_pBMPQiziBits =NULL;
    }
}

bool  CMatPlayer::BMPGo(CHESSMOVE cmQiJuMove, bool bBottomGo)   //ͼ����������
{
        POINT from,to;
        bool issuccess = false ;
        BYTE position[10][9];

        //assert(IsWindow(m_hWndBoard)&&IsWindow(m_hWndProg));
        //assert(m_iChessMoveFrom);  //m_iChessMoveFrom����ʱ���϶�����UCCI

        m_Move =cmQiJuMove ;
       // if( m_bProgBottom != bBottomGo ) //�����������棬���������£�ת���岽��ʽ�����治��
        {
            m_Move.From.x = 8 -m_Move.From.x ;
            m_Move.From.y = 9 -m_Move.From.y ;
            m_Move.To.x = 8 -m_Move.To.x ;
            m_Move.To.y = 9 -m_Move.To.y ;
         }

        from.x=  m_LRookInBoard.x + m_Move.From.x* m_dx;
        from.y=  m_LRookInBoard.y + m_Move.From.y* m_dy;
        to.x=  m_LRookInBoard.x + m_Move.To.x* m_dx;
        to.y=  m_LRookInBoard.y + m_Move.To.y* m_dy;

        if(IsIconic(m_hWndProg))
        {
                ShowWindow(m_hWndProg,SW_SHOW);
        }
//        BringWindowToTop( m_hWndProg );       //������һ�²�ͬ�ĺ���Ч��
//        SetForegroundWindow ( m_hWndProg );


        memcpy(position,m_position,90);        //����֮ǰ������������

        //���壬����������������������Ҫ
//	position[m_Move.To.y][m_Move.To.x] = m_position[m_Move.From.y][m_Move.From.x];
//	position[m_Move.From.y][m_Move.From.x] = NOCHESS;

        SendMessage(m_hWndBoard, WM_LBUTTONDOWN, 0, from.y<<16 | from.x & 0x0000ffff);
        SendMessage(m_hWndBoard, WM_LBUTTONUP, 0, from.y<<16 | from.x & 0x0000ffff);

        SendMessage(m_hWndBoard, WM_LBUTTONDOWN, 0, to.y<<16 | to.x & 0x0000ffff);
        SendMessage(m_hWndBoard, WM_LBUTTONUP, 0, to.y<<16 | to.x & 0x0000ffff);
        return true;

//         check if it is same as m_ChessBoard;
        GetBMPBoard();
        BMP2Position();
        for(int i = 0;i<10;i++)
         for(int j = 0;j<9 ;j++)
         {
             if( m_position[i][j]!= position[i][j] )
             {
                   issuccess = true;
                   break;
             }
         }
        if(IsIconic(m_hWndProg))
        {
          ShowWindow(m_hWndProg,SW_SHOWMINNOACTIVE);
        }
        return issuccess;
}

//
HWND CMatPlayer::FindWinProg(void)
{
    HWND   parent=NULL,brother=NULL;
    m_iLeftTop = m_LRookInProg;    //ͨ�� m_LRookInProg �ҵ����̴��ھ��
    char bufferclass[1024];
    char buffertitle[1024];
   //assert( m_MaxParentWinCountBoard >= 0 );

    strcpy(buffertitle,"CycloneGui �Ż��ߣ�����Ҧ");

//     һ�����ȸ��ݹؼ�����ö���������
    m_hWndProg = FindWindow(NULL, m_ptcName) ;//���������ң��ҵ��˸���
    if(m_hWndProg==NULL)//û�ҵ����ùؼ����ҡ�����Ƚ��鷳��Ҫö�ٴ��ڣ��ҵ����Ⲣ�Ƚ�
    {     parent= GetDesktopWindow();
          brother = GetWindow(parent,GW_CHILD);   //��������һ���Ӵ���
          while (brother != NULL)
          {
              GetClassName(brother, bufferclass, 256);		//��ô�������
              GetWindowText(brother, buffertitle, 256);		//��ô��ڱ���
              if(strstr(buffertitle,m_ptcName)!=NULL)		//���������ӱȽ������Ƿ���ͬ
              {
					m_hWndProg= brother;
					break;
              }
              brother = GetWindow(brother,GW_HWNDNEXT); //������һ���Ӵ���
          }
    }

    //�ҵ��˾���һ�£�������
    FlashWindow(m_hWndProg,true);
    if(IsIconic(m_hWndProg))
    {
         ShowWindow(m_hWndProg,SW_SHOWNORMAL);
    }

    return m_hWndProg;
}

HWND CMatPlayer::FindWinBoard(void)    //���ݹؼ��ֺʹ�����Ϣ�����ҳ��򴰿ں����̴���
{
    HWND   WndChild=NULL;
    char bufferclass[1024];
    char buffertitle[1024];
    //assert( m_MaxParentWinCountBoard >= 0 );
    strcpy(buffertitle,"CycloneGui �Ż��ߣ�����Ҧ");

//     һ�����ȸ��ݹؼ�����ö���������
    if(NULL==m_hWndProg)
    {
        m_hWndProg= FindWinProg();
		if(NULL==m_hWndProg)
		{
			return NULL;
		}
    }

//      ������θ���������Ϣ�ҵ��Ӵ���
    int i;
//    &m_ClassNameBoard[m_MaxParentWinCountBoard][0] //����Ƕ��㴰��������
    WndChild = m_hWndProg ;
    for(int i=m_MaxParentWinCountBoard-1; i>=0; i--)
    {
        WndChild = FindWindowEx(WndChild, NULL, &m_ClassNameBoard[i][0], &m_WinNameBoard[i][0]);
        if(WndChild!=NULL)
        {
            m_hWndBoard=WndChild;
        }
    }

    if(IsIconic(m_hWndProg))
    {
          ShowWindow(m_hWndProg,SW_SHOWNORMAL);
    }
   
	if(NULL==m_hWndBoard)  //û�ҵ������ң����������λ�ò��ң������ܲ�׼ȷ��
    {
		  BringWindowToTop( m_hWndProg );
		 ::ClientToScreen(m_hWndProg, &m_LRookInProg); //ת����m_iLeftTop to Screen ���������ָ��ڳ���λ�ã������Ļ���Ե�����
		 m_hWndBoard = WindowFromPoint(m_iLeftTop);
    }
	
	return m_hWndBoard;
}

bool CMatPlayer::LoadSolution(char* filename)
{
	if( GetFromFile(filename) && (FindWinProg()!=NULL) && (FindWinBoard()!=NULL) )
	{
		m_imgBoardInit = hwnd2mat(m_hWndBoard);
		CalcDxDy_CV();								//�������̸��
		PiecesMat();								//���������ͼ��
		return true;
	}
	return false;
}

//4�����ڽṹ�����ԣ�SSIM,structural similarity (SSIM) index measurement���ķ���
//�ṹ������������Ϊ����Ȼͼ���ź��Ǹ߶Ƚṹ���ģ������ؼ��к�ǿ������ԣ��ر��ǿ�������ӽ������أ�����������̺����Ӿ�����������ṹ����Ҫ��Ϣ��HVS����Ҫ�����Ǵ���Ұ����ȡ�ṹ��Ϣ�������öԽṹ��Ϣ�Ķ�����Ϊͼ���֪�����Ľ��ơ��ṹ������������һ�ֲ�ͬ������ģ��HVS�ͽ׵���ɽṹ��ȫ��˼�룬�����HVS���Եķ�����ȣ������������Զ��������Ե����ϵ�������һ��˼��Ĺؼ��ǴӶԸ�֪���������Ը�֪�ṹʧ�������ת�䡣��û����ͼͨ���ۼ�����������ѧ����֪ģʽ�йص����������ͼ������������ֱ�ӹ����������ӽṹ�źŵĽṹ�ı䣬�Ӷ���ĳ�̶ֳ����ƿ�����Ȼͼ�����ݸ����Լ���ͨ��ȥ��ص�����.��Ϊ�ṹ���������۵�ʵ�֣��ṹ���ƶ�ָ����ͼ����ɵĽǶȽ��ṹ��Ϣ����Ϊ���������ȡ��Աȶȵģ���ӳ����������ṹ�����ԣ�����ʧ�潨ģΪ���ȡ��ԱȶȺͽṹ������ͬ���ص���ϡ��þ�ֵ��Ϊ���ȵĹ��ƣ���׼����Ϊ�ԱȶȵĹ��ƣ�Э������Ϊ�ṹ���Ƴ̶ȵĶ�����
//������scalar��ʽ���棬��ȡscalar�ڵ����ݼ��ɻ�ȡ��Ӧ�����ƶ�ֵ������ֵ�ķ�Χ��0��1֮�䣬1Ϊ��ȫһ�£�0Ϊ��ȫ��һ����
double CMatPlayer::Similiar( const Mat& i1, const Mat& i2)
{
	assert(i1.rows==i2.rows && i1.cols==i2.cols);			//ͼ���Сһ�����ܱȽ�������

	const double C1 = 6.5025, C2 = 58.5225;
	//***************************** INITS **********************************/
	int d     = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d);           // cannot calculate on one byte large values
	i2.convertTo(I2, d);

	Mat I2_2   = I2.mul(I2);        // I2^2
	Mat I1_2   = I1.mul(I1);        // I1^2
	Mat I1_I2  = I1.mul(I2);        // I1 * I2

	//*************************** END INITS **********************************/

	Mat mu1, mu2;   // PRELIMINARY COMPUTING
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2   =   mu1.mul(mu1);
	Mat mu2_2   =   mu2.mul(mu2);
	Mat mu1_mu2 =   mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	///////////////////////////////// FORMULA ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

	Scalar mssim = mean( ssim_map ); // mssim = average of ssim map

	return (mssim.val[0] + mssim.val[1] + mssim.val[2]) /3;
}

/**************************************************************************************************
 * �������ƣ�Similiar()
 * ����:
 *      POINT   DestPt          - Ŀ���������Ͻǵ�
 *      BYTE *  pDstBMP         - Ŀ�����̴���λ����ָ��
 *      POINT   SrcPt           - Դ�������Ͻǵ�
 *      BYTE *  pSrcBMP         - Դ���̴���λ����ָ��
 * ����ֵ:
 *      unsigned long int       - �����������ӵ����ƶȡ�
 * ˵��:
 *      �������Ӵ�С�Ƚ���ȣ�������ܳ���
 *      �Ѱ�����ת����DIB����256ɫ��ÿλ����һ���ֽڣ�������㡣
 **************************************************************************************************/
unsigned long int  CMatPlayer::Similiar(POINT DestPt, BYTE * pDstBMP,POINT SrcPt, BYTE * pSrcBMP )
{
     assert(pSrcBMP!=NULL);
     assert(pDstBMP!=NULL);

     unsigned long int sumdd;
     BYTE     rgb;

     UINT bmWidthBytes=((m_bm.bmWidth *COLORBITSCOUNT+31)/32)* 4;    //DIB��ʽ
     // �����DDE���ݸ�ʽ����bmWidthBytes = m_bm.bmWidthBytes

     sumdd = 0 ;
     int i ,j;
     for( i  = 0  ; i < m_width ;     i++ )
     for( j  = 0  ; j < ( m_width*COLORBITSCOUNT/8 ) ;     j++ )     // �����DDE���ݸ�ʽ//������m_bm.bmBitsPixel����COLORBITSCOUNT
     {
          rgb=   pSrcBMP[ (SrcPt.y+i)* bmWidthBytes + SrcPt.x*COLORBITSCOUNT/8  +j ] ;
          rgb -= pDstBMP[ (DestPt.y+i)* bmWidthBytes + DestPt.x*COLORBITSCOUNT/8  +j ] ;
          if(rgb>0)
                  sumdd  =sumdd+ rgb;
          else
                  sumdd  =sumdd- rgb;
     }
     return sumdd;
}
//
unsigned int CMatPlayer::CalcDxDy()     //�ҵ���һ�������������DX��DY
{
     POINT  ptLeftCar,ptRightCarPerhaps;

     ptLeftCar.x = m_LRookInBoard.x -m_width /2 ;
     ptLeftCar.y = m_LRookInBoard.y -m_width /2 ;

     unsigned long int imin=0xffffffff;//�����
     unsigned long int imax=0x0;
     unsigned long int iRV=0;         //��λͼ���ƶ�ϵ������СֵΪ������
     long curdx,curdy;

    //
    for(int ii = ptLeftCar.x + m_width *7 ;ii< m_BoardWinRect.right - m_BoardWinRect.left - m_width ;ii++ )
     {
          // assert(ii<m_bm. );
           ptRightCarPerhaps.x = ii;
           ptRightCarPerhaps.y = ptLeftCar.y ;
            
           iRV=Similiar( ptRightCarPerhaps ,m_pBoardBitmapBits,ptLeftCar,m_pBoardBitmapBits) ;
           curdx = (ptRightCarPerhaps.x -ptLeftCar.x )/8 ;

           if( iRV<imin)
           {
                imin=iRV;
                m_dx=curdx ;
           }
           if ( iRV>imax)
           {
                imax=iRV;
           }
     }
     imax;
     imin;

     m_dy=m_dx;//�ٶ������ͬ
     return m_dx;
}

/**************************************************************************************************
 * �������ƣ�BMP2Position()
 * ����:
 *      HBITMAP hbitmap        - ���̴���λͼ���
 * ����ֵ:
 *      unsigned char*         - �ɹ�����ָ����������ָ�룬���򷵻�NULL��
 * ˵��:
 *      �ú�����Ҫ���������̴���λͼ����õ�������������,��Ҫ�취�ǣ�
 *      1������ʱ��������ڿ��У����뵱ǰ����λͼ����λ
 *      2�����ÿһ������λ�õ����ÿ�����ӣ����ƥ�䣬��λ�þ��Ǹ�����
 *         ÿ�����ӵ�ԭʼλͼ����λ������m_pBMPData�У�m_pBMPDataΪ��������ʱ�����ԭʼ������������
 **************************************************************************************************/
unsigned char*   CMatPlayer::BMP2Position(void)
{
//     1������ʱ��������ڿ��У����뵱ǰ����λͼ����λ
       if(SaveBoardBits(m_pCurBMPBoardBits)){
       }

       //     2�����ÿһ������λ�õ����ÿ�����ӣ����ƥ�䣬��λ�þ��Ǹ�����
//        ÿ�����ӵ�ԭʼλͼ����λ������m_pBMPData��
//        ƥ��ı�׼�������ƶ�С��һ����ֵ  THRESHOLD=100
     POINT  src,dest,ooo;

     ooo.x = m_LRookInBoard.x -m_width /2 ;         //m_LRookInBoard�ڳ����ĵ���������̵�λ�ã��൱������ԭ�㣬to Board client
     ooo.y = m_LRookInBoard.y -m_width /2 ;         //ooo�ڳ��󶥵���������̵�λ�á�

     for(int u=0;u<10;u++)
      for(int v=0;v<9;v++)
      {
         m_position[u][v] = 0;
         src.x = ooo.x + v * m_dx;
         src.y = ooo.y + u * m_dy ;

         unsigned long int imin=0xfffff;//�����
         unsigned long int imax=0x0;
         unsigned long int iRV=0;
         int curm=0;

         for( int m =1 ;m<15; m++ )
         {
              dest.x = ooo.x + m_InitPosOfPieceType[m].x * m_dx;
              dest.y = ooo.y + m_InitPosOfPieceType[m].y  * m_dy;
              iRV=Similiar( dest, m_pBoardBitmapBits, src,m_pCurBMPBoardBits) ;
              curm = m ;
              //����ͳ�������Сֵ���ڵ��ԣ��ҵ����ƶ���ֵ
              if( iRV<imin) {
                 imin=iRV;
                 if(iRV<THRESHOLD) {
                          m_position[u][v] = curm ;
                 }
              }
              if ( iRV>imax){
                   imax=iRV;
              }
          }
      }
      //ҪУ��m_position����ȷ�ԣ�����ȷ����NULL��

     assert(   m_pCurBMPBoardBits!=NULL);
     delete [] m_pCurBMPBoardBits ;
     m_pCurBMPBoardBits=NULL;

     if(ValidPosition(NULL))
     {
        return &m_position[0][0] ;  // may be incorrect due to local var.!!!
     }  else
     {
        //Application->MessageBox("catch!","ValidPosition error!",0);
       return NULL;
     }
}

//ҪУ��m_position����ȷ�ԣ�����ȷ����NULL��
bool   CMatPlayer::ValidPosition(BYTE position[10][9])
{    bool bOK=true;   BYTE chessID=NOCHESS;
    int qizinum[15]; //���ӵ���Ŀ

    ::ZeroMemory(qizinum, sizeof(int)*15);

      //for(int u=0;u<10;u++)
      //for(int v=0;v<9;v++)
      //{
      //      chessID=position==NULL?m_position[u][v]:position[u][v];
      //      if(chessID>R_PAWN)  bOK=false;
      //      qizinum[chessID]++;
      //}
      //if(qizinum[R_KING]!=1||qizinum[B_KING]!=1) bOK=false; //�������ҽ���һ������˧�����򵥵��ж�
    return bOK;
}

bool  CMatPlayer::GetBMPBoard()
{
        HDC             hscrdc, hmemdc;             // ��Ļ���ڴ��豸������
        HBITMAP         holdbitmap;                 // λͼ���
        int             nwidth, nheight;            // λͼ��Ⱥ͸߶�
        int             xscrn, yscrn;               // ��Ļ�ֱ���

        if(m_hWndBoard == NULL || !IsWindow(m_hWndBoard))   {
             return false;
        }

        GetWindowRect(m_hWndBoard, &m_BoardWinRect);
        //if ( IsRectEmpty(m_BoardWinRect) )        // ȷ��ѡ������Ϊ�վ���
        //     return false;

        nwidth = m_BoardWinRect.right - m_BoardWinRect.left;     //
        nheight = m_BoardWinRect.bottom - m_BoardWinRect.top;

        // �����Ļ�ֱ���
//        xscrn = GetDeviceCaps(hscrdc, HORZRES );
//        yscrn = GetDeviceCaps(hscrdc, VERTRES );

        hscrdc = GetDC(m_hWndBoard);            //Ϊwindow�����豸������
        hmemdc = CreateCompatibleDC(hscrdc);    //Ϊwindow�豸�����������ݵ��ڴ��豸������

        if(m_hbitmap!=NULL) {
            DeleteObject(m_hbitmap);  m_hbitmap=NULL;
        }
        m_hbitmap = CreateCompatibleBitmap(hscrdc, nwidth, nheight);// ����һ������Ļ�豸��������ݵ�λͼ
        holdbitmap = (HBITMAP)SelectObject(hmemdc, m_hbitmap);     // ����λͼѡ���ڴ��豸��������
        BitBlt(hmemdc, 0, 0, nwidth, nheight,hscrdc, 0, 0, SRCCOPY);// ��window�������ڴ�
        m_hbitmap = (HBITMAP)SelectObject(hmemdc, holdbitmap);     //�õ�windowλͼ�ľ��

        ReleaseDC(m_hWndBoard,hscrdc);     //���
        DeleteDC(hmemdc);

        return m_hbitmap==NULL?false:true;       // ����λͼ���
}

bool     CMatPlayer::SaveBoardBits(HBITMAP hbitmap)
{
//   ����λͼ��Ϣ���BITMAP�ṹ���ֶΣ�������bmBits�ֶε���NULL
     GetObject(hbitmap, sizeof(BITMAP), (BYTE *)&m_bm );
//     typedef struct tagBITMAP {  /* bm */
//    int     bmType;//������BM
//    int     bmWidth;//ָ��λͼ�Ŀ�ȣ�������Ϊ��λ��
//    int     bmHeight;//ָ��λͼ�ĸ߶ȣ�������Ϊ��λ����
//    int     bmWidthBytes;//һ�м����ֽڣ�4λ����
//    BYTE    bmPlanes;//ָ��Ŀ���豸��λ����
//    BYTE    bmBitsPixel;//ָ��ÿ�����ص�λ��
//    LPVOID  bmBits;//ָ��ͼ�����ݵ�ָ��
//} BITMAP;

     DWORD dwBitmapSize = (DWORD)m_bm.bmWidthBytes* m_bm.bmHeight * m_bm.bmPlanes;

     if(m_pBoardBitmapBits!=NULL)
     {
         delete m_pBoardBitmapBits;
         m_pBoardBitmapBits = NULL;
     }
     int wBitCount=24;    //bmBitsPixel
     dwBitmapSize = ((m_bm.bmWidth *wBitCount+31)/32)* 4*m_bm.bmHeight ;
     m_pBoardBitmapBits = new (std::nothrow) BYTE[dwBitmapSize];
     if(m_pBoardBitmapBits==NULL)return false;
     //  ��ȡͼ��������Ҫʹ��GetBitmapBits
     GetBitmapBits(hbitmap, dwBitmapSize, m_pBoardBitmapBits);      //the second BMP
     return true;
}

CHESSMOVE Str2Move(const char str[5])
{
    CHESSMOVE mv;
    mv.ChessID=0;
    mv.From.x=0;
    mv.From.y=0;
    mv.To.x=0;
    mv.To.y=0;
    if(strlen((char *)str)==4)
    {
        mv.From.x=str[0]- 'a';
        mv.From.y='9'-str[1];

        mv.To.x=str[2]- 'a';
        mv.To.y='9'-str[3];;
    }
    return mv;
}

CHESSMOVE  CMatPlayer::Position2Move(BYTE pre_position[10][9] ) //ͨ���Ƚ�����positionȡ���岽
{
        POINT   pt1,pt2 ,       from,to;
        BYTE    qizi1,          qizi2;
        BYTE    killed ,ChessID;
        killed = ChessID = NOCHESS;

        bool    srcblackup = false ;
        bool    destblackup = false ;

       for(int m=3;m<6;m++)
        for(int n=0;n<3;n++)
        {
         if(m_position[n][m]== B_KING )
         {
              srcblackup = true;
              break;
         }
        }
       for(int m=3;m<6;m++)
        for(int n=0;n<3;n++)
        {
           if(pre_position[n][m]== B_KING )
           {
             destblackup = true;
             break;
           }
        }
        if( srcblackup ^ destblackup )
        {
            ChangeSide( m_position );
        }

        pt1.x = pt1.y=pt2.x=pt2.y=-1;
        from.x = from.y=to.x=to.y=-1;
        for(int i =0;i<10;i++)
        for(int j =0;j<9;j++)
        {
           qizi2 = pre_position[i][j];
           qizi1 = m_position[i][j];
           if(qizi1 != qizi2 )      // where qizi moved
           {
                if( qizi1 == NOCHESS )
                {
                        from.x = j;     from.y=i;
                }
                else
                {
                        to.x= j ;       to.y=i;
                        if ( qizi2!= NOCHESS )
                        {
                                killed = qizi2;
                        }
                        else
                        {
                                killed = NOCHESS;
                        }
                        ChessID = qizi1;
                 }
           }
        }

        if( srcblackup ^ destblackup )
        {
//             from.x = 9- from.x;
//             from.y = 10- from.y;
//             to.x   = 9- to.x;
//             to.y   = 10-to.y;
             ChangeSide( m_position );
        }

        m_Move.From.x   = from.x;
        m_Move.From.y   = from.y;
        m_Move.To.x     = to.x;
        m_Move.To.y     = to.y;
        m_Move.KillID   = killed ;
        m_Move.ChessID  = ChessID;

        return m_Move;
}

//
CHESSMOVE CMatPlayer::GetMove(BYTE pre_position[10][9])
{
      char * p="#pragma warning( disable:4996 )";
      char buffer[1024];
      CHESSMOVE mvtemp;
        m_Move.From.x =-1;
        m_Move.From.y =-1;
        m_Move.To.x =-1;
        m_Move.To.y =-1;
        m_Move.ChessID =NOCHESS;
	  int m_iChessMoveFrom = BMP_PLAYER;
      switch(m_iChessMoveFrom)
      {
          case ENGINE_PLAYER:      //ͨ����������ȡ�岽

                //p = CNewQiJu::ToFen(MainForm->myqiju.m_ChessBoard,MainForm->myqiju.m_bBottomGo);
                StrToClip(p);
                
                strcpy(buffer,"position fen ");
                strcat(buffer,p);
                //strcpy(buffer,"position fen rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w");  //ԭʼ����

                m_ucci.UCCI2Engine(buffer);
                m_ucci.UCCI2Engine("go depth 5");

                while(m_ucci.ReceiveUCCI())
                {

                    if(4==strlen(m_ucci.mvPonder))
                    {
                      mvtemp= Str2Move(m_ucci.mvPonder);
                      strcpy(m_ucci.mvPonder,"move not ready!");
                      m_Move=mvtemp;

                      strcpy(m_ucci.mvPonderFinished,"move not ready!");
                   }
                }
                break;

          case  BMP_PLAYER:      //ͨ��ɨ�����̻���岽
                RECT  myrect;
                int m_width , height;

                m_width  = m_BoardWinRect.right -  m_BoardWinRect.left  ;
                height = m_BoardWinRect.bottom - m_BoardWinRect.top ;

                GetWindowRect(m_hWndBoard, &myrect);

                if(   myrect.right - myrect.left != m_width || myrect.bottom - myrect.top != height )
                {
                    m_width  = m_ProgWinRect.right - m_ProgWinRect.left  ;
                    height = m_ProgWinRect.bottom - m_ProgWinRect.top ;

                    MoveWindow(m_hWndProg, m_ProgWinRect.left, m_ProgWinRect.right ,m_width,height ,true );
                }

                //BringWindowToTop( m_hWndProg );
                try
                {

                    if( GetBMPBoard())
                    {
                          BMP2Position() ;
                          Position2Move(pre_position);  //�Ƚ�������������ȡ���岽

                          // ����岽��Ч��;
                          BYTE position[10][9];

                          memcpy(position,  m_position ,90);
                          mvtemp = m_Move;
                          if(IsValidMove( position,m_Move))
                          {
                              if( m_bQiShouRed != m_bQiShouBottom )   //�˴�Ҫ�����޸�
                              {
                                    ChangeSide( position);
                                    mvtemp.From.x = 8-mvtemp.From.x;
                                    mvtemp.From.y = 9-mvtemp.From.y;
                                    mvtemp.To.x = 8-mvtemp.To.x;
                                    mvtemp.To.y = 9-mvtemp.To.y;
                              }
                              m_Move=mvtemp;
                          }
                    }
                }
                catch (...)
                {
                    ErrorHappen("GetMove���������쳣��");
                }
                break;

          case EDIT_PLAYER:      //�ֶ������岽��������h0g2��=�·��������

              break;

          case VOICE_PLAYER:      //ͨ��������ȡ�岽
               break;

		  default:
			    break;
      }
      return m_Move;
}

void CMatPlayer::DrawCchessBorder(void)
{
	HPEN hPen ,* hOldPen ;
	if(m_hWndBoard == NULL || !IsWindow(m_hWndBoard))
	{
		return;
	}
	HDC hdc = ::GetWindowDC( m_hWndBoard ) ;
	SetROP2( hdc , R2_NOT ) ;
	hPen = CreatePen ( PS_SOLID , 3 , RGB( 0 , 0 , 0 ) ) ;
	hOldPen = ( HPEN * )SelectObject( hdc , hPen ) ;
	SelectObject ( hdc ,GetStockObject ( NULL_BRUSH ) ) ;
	POINT pt1,pt2;
	int dx,dy;
	dx=m_dx;
	dy=m_dy;

	pt1.x = m_LRookInBoard.x;
	pt1.y = m_LRookInBoard.y;
	pt2.x=pt1.x+8*dx;
	pt2.y=pt1.y+9*dy;
	for(int i =0;i<10;i++)
	{
		MoveToEx(hdc,pt1.x,pt1.y+i*dy,NULL);
		LineTo(hdc,pt2.x,pt1.y+i*dy);
	}
	for(int i =0;i<9;i++)
	{
		MoveToEx(hdc,pt1.x+i*dx,pt1.y,NULL);
		LineTo(hdc,pt1.x+i*dx,pt2.y);
	}
	SelectObject( hdc , &hOldPen ) ;
	DeleteObject( hPen ) ;
	::ReleaseDC ( m_hWndBoard , hdc ) ;
}

//        �����ͼ�οͻ��˷����������ٰ����������ݲ����ҵ���������״̬
//        char            m_ptcName[MAX_PLAYER_NAME];   //��������,�����ͼ�οͻ�����Ϊ���ڱ���ؼ���
//        POINT           m_LRookInProg ;             //����ڳ��򶥲㴰�ںڳ�����λ�õ� ��ע����m_LRookInBoard������
//        BYTE            m_width;						//�ֱ��� ����������ͼ��������=��
//        unsigned long int     m_iDD;					//���,���ƶȡ�
//        BYTE          * m_pBoardBitmapBits;           //ָ��λͼ���ݿ飨��ʼ�������ݣ�

//      ��������Ҳ���Ա�����Ϊ��һ��Ѱ�����̴��ڵİ취
//        int             m_MaxParentWinCountBoard;       //ͼ�����̴����丸���ڼ���
//        char            m_ClassNameBoard[10][254];      //ͼ�����̴����丸��������
//        char            m_WinNameBoard[10][254];        //ͼ�����̴����丸���ڱ���


//��C++�ļ���������д 2017.5.20. 
//���ڷ����ı��棬��ʵֻҪ�����㹻����Ϣ���ܹ��ҵ����򴰿ڼ�������ڳ�������̴��ڵ�λ�ü��ɣ�
//�����Ը��ݳ��򴰿�hwnd����λ��point������Ϣ���ó�ʼ��������������build_solution�������½�������
//���豣��ͼ�����ݺ󣬷��������ļ�С�����
//���ο�loadsolution������������Щ�����Ǳ��豣��Ϊ�����Ĳ���
bool CMatPlayer::SaveSolution(char * filename)
{
		std::ofstream oFile(filename);
		if (!oFile.is_open()){
			return false;
		}

        long byteswriten=0;
        char* ptitle="link";

        oFile.write(ptitle, sizeof("link"));//д���ļ���ʶ		byteswriten += sizeof("link");

        oFile.write(m_ptcName,  sizeof(char)*MAX_PLAYER_NAME);	byteswriten +=  sizeof(char)*MAX_PLAYER_NAME;
        oFile.write((char*)&m_LRookInProg, sizeof(POINT));	byteswriten +=   sizeof(POINT);
		oFile.write((char*)&m_LRookInBoard,     sizeof(POINT));			byteswriten +=   sizeof(POINT);
		oFile.write((char*)&m_width,     sizeof(BYTE));			byteswriten +=   sizeof(BYTE);
		oFile.write((char*)&m_iDD,     sizeof(unsigned long int));	byteswriten +=   sizeof(unsigned long int);
		oFile.write((char*)&m_dx,      sizeof(BYTE));			byteswriten +=   sizeof(BYTE);		//
		oFile.write((char*)&m_dy,      sizeof(BYTE));			byteswriten +=   sizeof(BYTE);
		oFile.write((char*)&m_iChessMoveFrom, sizeof(int));			byteswriten +=   sizeof(int);
		oFile.write((char*)&m_bm, sizeof(BITMAP));			byteswriten +=   sizeof(BITMAP);

		//д�봰����Ϣ����
		oFile.write((char*)&m_MaxParentWinCountBoard, sizeof(int));			byteswriten +=   sizeof(int);
		oFile.write((char*)&m_ClassNameBoard, sizeof(m_ClassNameBoard));			byteswriten +=   sizeof(m_ClassNameBoard);
		oFile.write((char*)&m_WinNameBoard, sizeof(m_WinNameBoard));			byteswriten +=   sizeof(m_WinNameBoard);

		DWORD dwBitmapSize=0;//����λͼ���ݳ���
        if( m_pBoardBitmapBits !=NULL )
        {
            dwBitmapSize = ((m_bm.bmWidth *COLORBITSCOUNT+31)/32)* 4*m_bm.bmHeight ;
			oFile.write((char*)m_pBoardBitmapBits, dwBitmapSize  );			byteswriten += dwBitmapSize;
        }

		oFile.write((char*)&dwBitmapSize, sizeof(DWORD)  );			byteswriten +=   sizeof(DWORD);
        byteswriten+= sizeof( long );

		oFile.write((char*)&byteswriten, sizeof( long ));			//���д�뱾�ļ�д���ֽ�����
		
		oFile.close();

		return true;
}

//��C++�ļ���������д 2017.5.20. 
//��savesolution���Ӧ��һ����д��һ���Ƕ�
//��������ȡ���ݺ󣬻���Ҫ������������loadsolution������
bool CMatPlayer::GetFromFile(char * filename)
{
	//if( (_access( filename, 0 )) != -1 ) //�ж��ļ����ڷ�

	std::ifstream iFile(filename);
	if (!iFile.is_open()){
		return false;
	}

	char buffer[10];			
	int Len32= sizeof(*this) ;
	long bytesread=0;

	iFile.read(buffer, sizeof("link"));
	if(strlen(buffer)!=strlen("link")||strcmp(buffer,"link")!=0){
		return false;
	}

	iFile.read( m_ptcName,		sizeof(char)*MAX_PLAYER_NAME);
	iFile.read((char*)&m_LRookInProg,		sizeof(POINT));										/////////////////////////
	iFile.read((char*)&m_LRookInBoard,        sizeof(POINT));
	iFile.read((char*)&m_width,        sizeof(BYTE));
	iFile.read((char*)&m_iDD,        sizeof(unsigned long int));
	iFile.read((char*)&m_dx,         sizeof(BYTE));
	iFile.read((char*)&m_dy,         sizeof(BYTE));
	iFile.read((char*)&m_iChessMoveFrom, sizeof(int));
	iFile.read((char*)&m_bm,         sizeof(BITMAP));
	
	//���봰����Ϣ����
	iFile.read((char*)&m_MaxParentWinCountBoard, sizeof(int));
	iFile.read((char*)&m_ClassNameBoard, sizeof(m_ClassNameBoard));
	iFile.read((char*)&m_WinNameBoard, sizeof(m_WinNameBoard));

	//�����ڴ�
	DWORD dwBitmapSize ;
	dwBitmapSize = ((m_bm.bmWidth *COLORBITSCOUNT+31)/32)* 4*m_bm.bmHeight ;
	if( m_pBoardBitmapBits != NULL  )
	{
		delete   m_pBoardBitmapBits ;
		m_pBoardBitmapBits =NULL;
	}
	m_pBoardBitmapBits = new(std::nothrow) BYTE [dwBitmapSize];
	iFile.read((char*)m_pBoardBitmapBits ,dwBitmapSize);
	
	iFile.read((char*)&dwBitmapSize ,sizeof(DWORD));
	iFile.read(buffer ,sizeof(long));

	int i=  atoi(buffer);//�ж�һ������������long �Ƿ����ʵ�ʶ�ȡ�����ֽ���
	
	//assert(atoi(buffer)==bytesread);

	iFile.close();

	return true;
}


/*************************************************************************
 * �������ƣ�GetQiziDDB()
 * ����:
 *      HWND dstHWND        - ������ھ��
 *      unsigned short ChessID    - ����ID
 * ����ֵ:
 *      BYTE*              - �ɹ�����ָ������DDB���ݵ�ָ�룬���򷵻�NULL��
 * ˵��:
 *      �ú�����Ҫ����������DDB�������ҵ���Ӧ���ӣ�ChessID����DDB���ݡ�
 ************************************************************************/
BYTE* CMatPlayer::GetQiziDDB(HWND dstHWND,unsigned short ChessID)
{
    assert(m_hbitmap!=NULL&&m_pBoardBitmapBits!=NULL);
    assert(m_hbitmap!=NULL);
    assert(m_pBoardBitmapBits!=NULL);
    assert(dstHWND!=NULL);
    assert(m_pBoardBitmapBits!=NULL);
    assert(ChessID<=R_PAWN&&ChessID>=B_KING);

    POINT  ptQiziLeftTop,ooo;
    BYTE byQiziBits;

//    int iDI=atoi(MainForm->edtrdi->Text.c_str());
    int iDI=m_width;      //����������ȡ��ע�ͼ���

    ooo.x = m_LRookInBoard.x -iDI /2 ;   //�������Ͻ�
    ooo.y = m_LRookInBoard.y -iDI /2 ;

    ptQiziLeftTop.x = ooo.x + m_InitPosOfPieceType[ChessID].x * m_dx;     //�������Ͻ�
    ptQiziLeftTop.y = ooo.y + m_InitPosOfPieceType[ChessID].y * m_dy ;

    DWORD dwBitmapSize = (DWORD) ( iDI*m_bm.bmBitsPixel/8 )*iDI;
    if( m_pBMPQiziBits != NULL  )
    {
          delete   m_pBMPQiziBits ;
          m_pBMPQiziBits =NULL;
    }
    m_pBMPQiziBits = new BYTE [dwBitmapSize];

    for( int yDi  = 0  ; yDi < iDI ;     yDi++ )     //��������ѭ��
    for( int xbytes  = 0  ; xbytes < ( iDI*m_bm.bmBitsPixel/8 ) ;     xbytes++ )  //�����ֽ�ѭ���� ( iDI*m_bm.bmBitsPixel/8 )��ÿ�����ض��ٸ��ֽڣ�
    {
        m_pBMPQiziBits[yDi*( iDI*m_bm.bmBitsPixel/8 )+xbytes] = m_pBoardBitmapBits[ (ptQiziLeftTop.y+yDi)*m_bm.bmWidthBytes + ptQiziLeftTop.x*m_bm.bmBitsPixel/8  +xbytes];
    }

    HDC srcDC,dstDC;
    HBITMAP hOldbitmap,hBitmap;

    dstDC=GetDC(dstHWND);
    srcDC=CreateCompatibleDC(dstDC);

    hBitmap= CreateCompatibleBitmap(dstDC,iDI,iDI);
//    long int len=0;
    SetBitmapBits(hBitmap,dwBitmapSize,m_pBMPQiziBits);

    ReleaseDC(dstHWND,dstDC);
    DeleteDC(srcDC);
    return m_pBMPQiziBits;
}

//��ȡ���λ�õ���ײ�ؼ�,����disabled control
BOOL GetRealWindowFromPoint(HWND cHwnd);
BOOL GetRealWindowFromPoint(HWND cHwnd)
{
     POINT point,WindowPos={0};
     GetCursorPos(&point);
     ClientToScreen(cHwnd,&WindowPos);//ת������Ļ����,��ʾ�ͻ����������Ͻǵ�����
     point.x-=WindowPos.x;
     point.y-=WindowPos.y;
     HWND wmControl=ChildWindowFromPoint(cHwnd,point);//�ͻ�������

    // getlasterr(GetLastError());
     if (wmControl!=NULL)
     {
          if(wmControl!=cHwnd)//wmControl==cHwndʱ��ʾ�Ѿ�����RealChildWindowFromPoint����ȡ������ײ�
           GetRealWindowFromPoint(wmControl);//�ݹ�

          else
          {
        //   ::GetControlInfo(wmControl);
           return TRUE;
          }
     }
     return true;
}


void ErrorHappen(LPTSTR lpszFunction)
{
    char szBuf[80];
    LPVOID lpMsgBuf;
    DWORD dw = ::GetLastError();
 
    ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (LPTSTR) &lpMsgBuf,0, NULL );
 
    sprintf(szBuf, "%s failed with error 0x%08u: %s",  lpszFunction, dw, lpMsgBuf);

    if(dw)
    {
        ::MessageBox(NULL,szBuf,NULL,1);
    }

    ::LocalFree(lpMsgBuf);
}

// ��DDBλͼ����������DIB��ʽ���浽�β�pdataָ��λ�ã�pdata������ʱ����ɾ��
// ��ɫλ���̶�Ϊ8λ���Է������
// ��Ҫʱ����Ϊ�ļ����ɼ��
bool  CMatPlayer::SaveBoardBits(BYTE* &pdata)
{
//        assert(m_hbitmap!=NULL);
//        assert(pdata!=NULL);
//        if(pdata==NULL)return false;
        if(m_hbitmap==NULL)return false;

//        assert(m_hbitmap!=NULL);
        if(m_hbitmap==NULL)return false;

	HDC hDC;        //�豸�������
	int iBits;        //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;   //λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С
	DWORD  dwPaletteSize=0,   dwBmBitsSize,	   dwDIBSize, dwWritten;
	BITMAPFILEHEADER bmfHdr;        //DIBλͼ�ļ�ͷ�ṹ
	//BITMAP	 m_bm;                //DDBλͼ���Խṹ    ��Ϊ��Ա����
	BITMAPINFOHEADER	bi;     //λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;        //ָ��λͼ��Ϣͷ�ṹ
	HANDLE   fh, hDib;              //�����ļ��������ڴ���
	HPALETTE hPal,hOldPal=NULL;     //��ɫ����

	//���ݵ�ǰ��Ļ����λͼ�ļ�ÿ��������ռλ����Ҳ����������Ϊ1��4��8��24��λ��
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else wBitCount = 24;

        wBitCount=COLORBITSCOUNT;         //�̶�Ϊ8λ��������㡣

	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);

	//��ȡDDBλͼ��Ϣ
	GetObject(m_hbitmap, sizeof(BITMAP), (LPSTR)&m_bm);

	//����DIBλͼ��Ϣͷ�ṹ
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = m_bm.bmWidth;
	bi.biHeight          = -m_bm.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((m_bm.bmWidth *wBitCount+31)/32)* 4*m_bm.bmHeight ;


	//Ϊλͼ���ݷ����ڴ�
	//hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	hDib  = GlobalAlloc(GHND,dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
        //lpbi = (LPBITMAPINFOHEADER) new BYTE(sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize);   //�����ַ�ʽ�������ڣ�����GetDibbits�ò������ݣ���֪�ιʣ�

	*lpbi = bi;   //�����ڴ���BI��Ϣ�ṹ


	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	if (hPal) {
		hDC  = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, hPal, FALSE);
		RealizePalette(hDC);
	}

	//��ȡ�õ�ɫ�����µ�����ֵ ��λͼ����ɫ��������BITMAPINFO�ṹ�ĺ���
        //GetDIBits������ȡָ������λͼ��λ��Ȼ������һ��DIB���豸�޹�λͼ��Device-Independent Bitmap��ʹ�õ�ָ����ʽ���Ƶ�һ���������С�
        //�������Ҫ��DIB��ʽ�����ڲ���ʽ��ƥ�䣬��ôλͼ��RGBֵ�������ơ������ƥ�䣬��ô���ϳ�һ����ɫ��
        //ͨ�����߶���Ϊ������ָ��һ�����¶��ϵ�DIB�������϶��µ�DIB��ͨ������һ�����ĸ߶�ֵ��ָ����
        //λͼ����ɫ��������BITMAPINFO�ṹ�ĺ���
        //
       UINT scanlines=0;
       //scanlines=GetDIBits(hDC, m_hbitmap, 0, (UINT)m_bm.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

       //SaveBitmapToFile("BoardForTest.bmp");      //����ʱ�� ������鱣���ͼ��Բ���

       if(pdata!=NULL)  {
         delete []pdata;
         pdata=NULL;
       }
       pdata = new(std::nothrow) BYTE[dwBmBitsSize] ;

       if(pdata==NULL) {
            ErrorHappen("new(std::nothrow) BYTE[dwBmBitsSize] �������ִ��� ");
            return NULL;
       }
       //::ZeroMemory(pdata, dwBmBitsSize);
       scanlines=GetDIBits(hDC, m_hbitmap, 0, (UINT)m_bm.bmHeight,pdata,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
       //memcpy(pdata,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,dwBmBitsSize);

	//�ָ���ɫ�� �����Գɹ������ɾ�����ɫ����صĴ������ԣ�
	if (hOldPal) 	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);

        //delete []lpbi;
        
	return TRUE;
}


// ��DDBλͼ��DIB��ʽ�浽�����ļ���.
// lpFileName Ϊλͼ�ļ���������ͨ����
bool  CMatPlayer::SaveBitmapToFile(LPSTR lpFileName)
{
//        assert(m_hbitmap!=NULL);
        if(m_hbitmap==NULL)return false;

	HDC hDC;        //�豸�������
	int iBits;        //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;   //λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С
	DWORD  dwPaletteSize=0,   dwBmBitsSize,	   dwDIBSize, dwWritten;
	BITMAPFILEHEADER bmfHdr;        //DIBλͼ�ļ�ͷ�ṹ
	BITMAP	 Bitmap;                //DDBλͼ���Խṹ
	BITMAPINFOHEADER	bi;     //λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;        //ָ��λͼ��Ϣͷ�ṹ
	HANDLE   fh, hDib;              //�����ļ��������ڴ���
	HPALETTE hPal,hOldPal=NULL;     //��ɫ����

	//���ݵ�ǰ��Ļ����λͼ�ļ�ÿ��������ռλ����Ҳ����������Ϊ1��4��8��24��λ��
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else wBitCount = 24;

        wBitCount=8;//�޸�Ϊ8λ��Σ������á�

	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);

	//��ȡDDBλͼ��Ϣ
	GetObject(m_hbitmap, sizeof(BITMAP), (LPSTR)&Bitmap);

	//����DIBλͼ��Ϣͷ�ṹ
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = -Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;


	//Ϊλͼ���ݷ����ڴ�
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;   //�����ڴ���BI��Ϣ�ṹ

	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	if (hPal) {
		hDC  = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, hPal, FALSE);
		RealizePalette(hDC);
	}

	//��ȡ�õ�ɫ�����µ�����ֵ ��λͼ����ɫ��������BITMAPINFO�ṹ�ĺ���
	UINT scanlines=GetDIBits(hDC, m_hbitmap, 0, (UINT)Bitmap.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ��
	if (hOldPal) 	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//�����ļ�
	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)  return FALSE;

	//����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)	+ dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize;
	//д���ļ�
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

void StrToClip(char* pstr) //����FEN����������
{
    if(::OpenClipboard (NULL))//�򿪼�����
     {
          HANDLE hClip;
          char* pBuf;
          ::EmptyClipboard();//��ռ�����

        //д������
          hClip=::GlobalAlloc(GMEM_MOVEABLE,strlen(pstr)+1);
          pBuf=(char*)GlobalLock(hClip);
          strcpy(pBuf,pstr);
          ::GlobalUnlock(hClip);//����
          ::SetClipboardData(CF_TEXT,hClip);//���ø�ʽ

        //�رռ�����
          ::CloseClipboard();
     }
}

void  CMatPlayer::ChangeSide(BYTE position[10][9])
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


bool CMatPlayer::IsValidMove(BYTE position[10][9],CHESSMOVE mv)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
//�������̳�ʼͼ������λ�õȲ������ֽ��14������ͼ��ȱʡ�߿�40��
//ע�⣺δ��ʵ�������ݣ���Ҫ����m_imgBoardInit���ݲ��䣡
bool CMatPlayer::PiecesMat()
{
	assert(m_imgBoardInit.rows > m_width*5);

	string fn;
	for(int y=0; y<10 ; y++)		
	{
		for(int x=0; x<5 ; x++)//ֻȡ��ߵ�
		{
			if(int ChessID = InitChessBoard[y][x])
			{
				m_imgPiece[ChessID] = m_imgBoardInit( Rect(m_LRookInBoard.x+x*m_dx -m_width/2, m_LRookInBoard.y+y*m_dy-m_width/2, m_width, m_width) );
				fn ="res\\Piece"+ nameID[ChessID]+".jpg";
				//imwrite(fn, m_imgPiece[ChessID]);
			}
		}
	}
	
	Mat temp=m_imgPiece[1];	
	for(int x=2; x<=14 ; x++)	
	{
		temp = mergeCols(temp, m_imgPiece[x]);
	}
	imwrite("res\\allPiece.jpg", temp);

	return true;
}

//�ҵ���һ�����������DX�����������ͼ
//�ٶ�dy=dx�����Բ���ȵ����δ�����⴦��
unsigned int CMatPlayer::CalcDxDy_CV(void)
{
	double rMax=0;

	Mat rRook;
	Mat lRook = m_imgBoardInit(Rect(m_LRookInBoard.x-m_width/2, m_LRookInBoard.y-m_width/2, m_width, m_width));
	imwrite("lRook.jpg", lRook);

	int RookX;
	double r;
	for(int x = m_LRookInBoard.x+8*m_width; x < m_imgBoardInit.cols-m_width/2 ; x++)		//�����ٵ� 8���ӵ�x��ʼ���ɽ�ʡ��������
	{
		rRook = m_imgBoardInit(Rect(x - m_width/2, m_LRookInBoard.y-m_width/2, m_width, m_width));

		r= Similiar(lRook,rRook);

		if(r>rMax) 
		{
			rMax=r;
			RookX = x;
		}
	}

	//������ƶȳ�����ֵ���϶�Ϊ�ҳ����ݴ˼���dx,dy���������ͼ
	if(rMax>m_threshold)
	{
		m_dx=(RookX-m_LRookInBoard.x)/8;
		m_dy=m_dx;

		cout<<  "ssim:  " << rMax << endl;
		cout<<  "m_dx:  " << m_dx << endl;
		imwrite("rRook.jpg",rRook);

		PiecesMat();
	}

	return m_dx;
}

//���ȼ�������ʵʱ���꽨����ʼͼ�����߷������������m_dx��������positonʶ��
//ֻ��Ҫһ����Ļ����Ϳ����ˣ����ָ���󳵣������ȼ��󣬴���������Ļ�������������ô˺�������
bool CMatPlayer::build_solution(POINT MousePoint)
{
	//::GetCursorPos(&MousePoint);
	
	m_hWndBoard = ::WindowFromPoint(MousePoint);
	::GetWindowRect(m_hWndBoard, &m_BoardWinRect);

	// �ռ�������Ϣ
	// 1����ǰ����
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<254;j++)  
		{
			m_ClassNameBoard[i][j]=0;
			m_WinNameBoard[i][j]=0;
		}
	}

	::GetWindowText( m_hWndBoard ,&m_WinNameBoard[0][0], 254 );  //���̴��ڵ���Ϣ
	::GetClassName( m_hWndBoard ,&m_ClassNameBoard[0][0], 254 );

	// 2��locate top window
	m_MaxParentWinCountBoard=0;
	m_hWndProg = GetParent(m_hWndBoard) ;
	while( m_hWndProg!= NULL)
	{
		m_MaxParentWinCountBoard++;
		::GetWindowText( m_hWndProg ,&m_WinNameBoard[m_MaxParentWinCountBoard][0], 254 );
		::GetClassName( m_hWndProg ,&m_ClassNameBoard[m_MaxParentWinCountBoard][0], 254 );
		m_hWndProg = ::GetParent(m_hWndProg)  ;
	}

	m_iLeftTop = MousePoint;
	m_LRookInProg = MousePoint;
	m_LRookInBoard = MousePoint ;
	strcpy( m_ptcName, &m_WinNameBoard[m_MaxParentWinCountBoard][0]);	//���ڿɱ䴰�ڱ��⣬��Ҫ���ùؼ������������

	//���½����������漰����Ա������m_imgBoardInit��m_LRookInBoard��ʵʱͼm_imgBoard������ʶ��ģ�
	//�ú���Ӧ��hWnd������ʶ�Ĵ��ں�POINT�ṹ��������Ļ�����������û����꣬
	//Ȼ�����û������������Ļ���꣬�������������ָ�����ڵ���������Ͻ�
	::ScreenToClient( m_hWndProg, &m_LRookInProg);
	::ScreenToClient( m_hWndBoard,&m_LRookInBoard);

	//��������
	m_imgBoardInit = hwnd2mat(m_hWndBoard);		//����ԭʼ����ͼ��
	imwrite("BoardInit.jpg",m_imgBoardInit);
	CalcDxDy_CV();								//��m_dx�����������ͼ������������ʱ��CalcDxDy_CVһ�Σ�ʶ��ʱ���ٵ���
	
	//ʶ��
	string fen = BMP2Position(m_bQiShouRed);					

	return true;
}

//����ԭʼ����������m_imgPiece[15]��ʵʱ����m_imgBoard�õ����fen��
//redGo���ֱ�־��true=����
//string CMatPlayer::BMP2Position(Mat imgboard, bool redGo, Mat imgBoardInit, POINT ptLRook)
string CMatPlayer::BMP2Position(bool redGo)
{
	double fSimiliar=0;
	double fMax=0;
	string fen="";
	int ChessID=0;
	int number=0;
	Mat imgtemp, imgPiece;
	bool bfound=false;
	
	if(m_hWndBoard!=NULL)
		m_imgBoard = hwnd2mat(m_hWndBoard);

	if( m_imgBoard.data == NULL ) return "";

	ZeroMemory(m_position, sizeof(m_position));

	for(int y=0; y<10 ; y++)
	{
		for(int x=0; x<9 ; x++)
		{
			imgtemp = m_imgBoard( Rect(m_LRookInBoard.x+x*m_dx -m_width/2, m_LRookInBoard.y+y*m_dy-m_width/2, m_width, m_width) );
			for(ChessID = 1; ChessID<15; ChessID++)
			{
				double fSimiliar= Similiar(imgtemp, m_imgPiece[ChessID]);
				if(fSimiliar>fMax) 
				{
					imgPiece = m_imgPiece[ChessID];
					fMax = fSimiliar;
					number=ChessID;
					bfound=true;
				}
			}

			if(fMax > m_threshold)
			{
				std::cout << "fMax " << std::setw(10) <<  fMax  << std::setw(10) << nameID[number]  << std::endl;
				fen ="res\\"+ nameID[number]+".jpg";
				imwrite(fen.c_str(), mergeCols(imgPiece, imgtemp));

				m_position[y][x] = number;

				fMax=0;
				number=0;
				bfound=false;
			}
		}
	}
	std::cout << std::endl;

	fen =ToFen(m_position, redGo);
	StrToClip((char*)fen.c_str());	//��fen��������ϵͳ��������

	return fen;
}

BYTE *  CMatPlayer::PasteJuMian(char FenStr[2054])
{
        const char PieceChar[15] = { '/','k', 'r', 'n', 'c', 'a', 'b', 'p' ,'K', 'R', 'N','C', 'A','B', 'P' };
        strcpy(FenStr, "2bakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 3 6");
        BYTE position[10][9];
        for(int i=0;i<10;i++)
        for(int j=0;j<9;j++)
        {
                position[i][j]='f';
        }
        int len = strlen(FenStr);
        char ChessID ='0';

        int y =0 , x=0;
        for(int i = 0;i<len ;i++)
        {
              ChessID = FenStr[i] ;
//              ID= ChessID -48;
              if( ChessID <='9' && ChessID >='1' )
              {

                  for(int j=x ;j< x+ ChessID -48;j++)
                  {
                       position[y][j] = NOCHESS;

                  }
                  x = x+ ChessID -48;
              }
              else if( ChessID <='z' && ChessID >='a' ||   ChessID <='Z' && ChessID >='A' )

              {
                  for(int m=0 ;m< 15; m++)
                  if( ChessID == PieceChar[m])
                  {
                      position[y][x] = m;
                      x++;
                      break;
                  }
              }
              else if(ChessID == '/')
              {
                  y++;
                  x=0;
              }
              if(y>9 || ChessID==' '|| x>9)
              {
                     break;
              }
        }
	return &position[0][0];
}

char*  CMatPlayer::ToFen(BYTE position[10][9] , bool RedGo )
{
        static char FenStr[2054];
        const char PieceChar[15] = { '/','k', 'r', 'n', 'c', 'a', 'b', 'p' ,'K', 'R', 'N','C', 'A','B', 'P' };
        strcpy(FenStr, "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 3 6");
        strcpy(FenStr, "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 3 6");
        memset(FenStr,0,2000);
		strcpy(FenStr, "");
        char * lines="/";
		// ��������
        int m =0;
		for(int i=0; i<10;  i++)
        {
              m =0;
              for(int j=0; j<9; j++)
              {
                      // "3akabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR b asdfdsafdsf- --- 56 67"
                      if(position[i][j])
                      {
                              if(m)
                              {
                                        char string[25];
                                        strcpy(string,"");
                                        string[1] = 0;
                                        strcat( FenStr, itoa( m, string, 10 ) );
                                        m=0;
                              }
                              char string[25];
                              strcpy(string,"");
                              string[0]= PieceChar[ position[i][j] ];
                              string[1] = 0;
                              strcat( FenStr , string );
                      }
                      else
                      {
                              m++;
                              if(j==8)
                              {
                                        char string[25];
                                        strcpy(string,"");
                                        string[1] = 0;
                                        strcat( FenStr, itoa( m, string, 10 ) );
                                        m=0;
                              }
                      }
              }
              if(i!=9)
              {
                strcat( FenStr ,lines );
              }
        }
        if(RedGo)
        {
              strcat( FenStr ," w - - 0 1");
        }
        else
        {
              strcat( FenStr ," b - - 0 1");
        }

	return FenStr;
}

//14��������ʼλ�ã������ߣ�
const POINT m_InitPosOfPieceType[15]={   {-1,-1}, 
									     {4,0},{0,0},{1,0},{1,2},{3,0},{2,0},{0,3},   //�ڷ�����
                                         {4,9},{0,9},{1,9},{1,7},{3,9},{2,9},{0,6}  };//�췽����
//��ʼ���̶���
const BYTE InitChessBoard[10][9]=
{
      { B_CAR,B_HORSE,B_ELEPHANT,B_BISHOP,B_KING,B_BISHOP,B_ELEPHANT,B_HORSE,B_CAR } ,
      { NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS}    ,
      { NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS}    ,
      { B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN}    ,
      { NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS}  ,
      { NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS}  ,
      { R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN}       ,
      { NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS}  ,
      { NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS}  ,
      { R_CAR,R_HORSE,R_ELEPHANT,R_BISHOP,R_KING,R_BISHOP,R_ELEPHANT,R_HORSE,R_CAR}
};



const string CMatPlayer::print(void) const 
{
	
	const  char ChessName[15][3]={"  ","��","��","��","��","ʿ","��","��","˧","��","��","��","��","��","��"} ; //����

	std::ostringstream ss;
	unsigned char txtboard[22][40]=
	{
		"     ----С���̴�ӡ ver 1.00!------- \n",
		"9 ����--��--��--��--��--��--��--��--�� ",
		". �����������������ܩ����������������� ",
		"8 ����--��--��--��--��--��--��--��--�� ",
		". ���������������������ܩ������������� ",
		"7 ����--��--��--��--��--��--��--��--�� ",
		". ������������������������������������ ",
		"6 ����--��--��--��--��--��--��--��--�� ",
		". ������������������������������������ ",
		"5 ����--��--��--��--��--��--��--��--�� ",
		". ���������������ӡ��������硡�������� ",
		"4 ����--��--��--��--��--��--��--��--�� ",
		". ������������������������������������ ",
		"3 ����--��--��--��--��--��--��--��--�� ",
		". ������������������������������������ ",
		"2 ����--��--��--��--��--��--��--��--�� ",
		". �����������������ܩ����������������� ",
		"1 ����--��--��--��--��--��--��--��--�� ",
		". ���������������������ܩ������������� ",
		"0 ����--��--��--��--��--��--��--��--�� ",
		". ���ᡡ�⡡�㡡�䡡�塡�桡�硡�衡�� ",
		"                                       "
	};

	for(int i=0; i<10;  i ++ )
	{
		for(int j=0; j<9; j ++ )
		{
			unsigned char ChessID = m_position[i][j];
			unsigned char BufferB[6]={"[��]"};

			if(ChessID!=0)
			{
				BufferB[1] = ChessName[ChessID][0];
				BufferB[2] = ChessName[ChessID][1];

				if( ChessID > 7 )
				{
					BufferB[0] ='(';	BufferB[3] =')';

				} else if( ChessID > 0 )
				{
					BufferB[0] =0x5b;	BufferB[3] =']';	
				}
				for(int len = 0 ;len < 4; len++)
				{
					txtboard[2*i+1][j*4+3+len]=BufferB[len];
				}
			}
		}
	}
	
	ss << "\n       --------���ڣ�" << __DATE__ << " -------\n ";
	for(int i=0; i<22;  i ++ )
	{
		ss<< "\n" << txtboard[i];
	}
	ss << "\nend!" << std::endl;

	return ss.str();
}