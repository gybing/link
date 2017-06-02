
/*******************************************************************************
������   : CMatPlayer
-------------------------------------------------------------------------------
����     : ���ݷ�����������ͼ�����fen����
		   1�������˹���λ��λ�õķ�ʽ����ͼ�����߷��������漰��ȡ������
		   2�����÷�����λͼ�οͻ������̴��ھ����
		   3���������̴��ھ��ʵʱ��ȡ����ͼ��
--------------------------------------------------------------------------------
�쳣��   : <��> 
--------------------------------------------------------------------------------
��ע     : 1��m_width�ɵ�����ʶ���������������ӻ����ֱ����ȷʶ��
		   2��
--------------------------------------------------------------------------------
�����÷� : 
	CUcci ucci;
	CMatPlayer bmpPlayer(ucci);
	//���½���������׼���������漰����Ա������m_imgBoardInit��m_LRookInBoard��m_imgPiece[]��
	bmpPlayer.m_LRookInBoard.x=52;
	bmpPlayer.m_LRookInBoard.y=62;
	bmpPlayer.m_imgBoardInit=imread("board.png");
	bmpPlayer.CalcDxDy_CV();				
	//ʵʱͼm_imgBoard������ʶ���
	bmpPlayer.m_hWndBoard=NULL;
	bmpPlayer.m_imgBoard=imread("board.png");
	//����ʶ�������������׼��fen�ִ���������������ճ������ͼ
	std::cout<< bmpPlayer.BMP2Position(true)<<std::endl;
--------------------------------------------------------------------------------
����     : <xxx>
*******************************************************************************/

#pragma once

#include <windows.h>

#include "types.h"
#include "ucciAdapter.h"

#include "opencv2/core/version.hpp"
#include "opencv2/core/core.hpp"

class CMatPlayer
{
  public:
        // CMatPlayer(char* winkeyname="CycloneGui");  //���ݴ��ڹؼ��ֻ��ļ�����ʼ��
        CMatPlayer(CUcci& ucci, std::string config="link.ini", BYTE width=40, double f=0.9);
        ~CMatPlayer();

        bool            SaveSolution(char * filename);
        bool            LoadSolution(char * filename);             //�ҵ������ڼ�������������
        HWND            FindWinProg(void);              //���ҳ��򴰿�
        HWND            FindWinBoard(void);             //�ҳ������̴���

        bool            BMPGo(CHESSMOVE cmQiJuMove, bool bBottomGo);   //ͼ����������

        bool            GetBMPBoard();                          //��ȡ����λͼ������λͼ���
        bool            SaveBoardBits(HBITMAP hbitmap);         //ͨ��Getbitmapbits������������λͼ����,��ԭʼ����ָ��
        bool            SaveBoardBits(BYTE* &pdata) ;           //ͨ��GetDibBits������������λͼ����,��ԭʼ����ָ��,�̶�256ɫ
        bool            SaveBitmapToFile(LPSTR lpFileName);     //��������
        BYTE *          BMP2Position(void);                     //����λͼ����ת��Ϊ��������

        CHESSMOVE       GetMove(BYTE pre_position[10][9] );     //pre_position:ǰһ�������������飨�Է����Ӻ��ҷ�δ��֮ǰ��
        void            DrawCchessBorder(void);     //

		//���������º������˽�г�Ա����
        unsigned int CalcDxDy() ;    //�ҵ���һ�������������DX��DY,ֻ��ԭʼʶ���ʱ�����һ��
        unsigned long int Similiar(POINT DestPt, BYTE * pDstBMP,POINT SrcPt, BYTE * pSrcBMP ); //���ƶȼ��㣬��������ƥ��,�������ƶ�
        BYTE*           GetQiziDDB(HWND dstHWND,unsigned short ChessID);//ȡ��ĳ���ӵ�λͼ���ݣ���߶�Ϊm_width

		//�����������ӵĺ�����
		bool	build_solution(POINT MousePoint);			//�����ȼ�ʱ, ����������ʵʱλ�ô�������
		std::string BMP2Position(bool redGo);		//�����������fen��ͬʱ������������

		static double	Similiar( const cv::Mat& i1, const cv::Mat& i2);	//�������ƶȵ���һ���汾���õ���opencv��
		unsigned int	CalcDxDy_CV();	//����m_dx����һ���汾���õ���opencv��
		bool			PiecesMat();	//��imgboard�зָ��15������ͼ�����m_imgPiece�����С�

		static void		ChangeSide(BYTE position[10][9]);
		static bool		IsValidMove(BYTE position[10][9],CHESSMOVE mv);
		static char *   ToFen(BYTE position[10][9] , bool RedGo );
		static BYTE *   PasteJuMian(char FenStr[2054]);
		const std::string	print(void) const ;

		//�������ݲ��֣�
		//�����ͼ�οͻ��˷����������ٰ����������ݲ����ҵ���������״̬
        char            m_ptcName[MAX_PLAYER_NAME];     //��������,�����ͼ�οͻ�����Ϊ���ڱ���ؼ���
        POINT           m_LRookInProg ;					//����ڳ��򶥲㴰�ںڳ�����λ�õ� ��ע����m_LRookInBoard������
        BYTE            m_width;						//�ֱ��� ���������ӿ�=��
        unsigned long int m_iDD;						//���,���ƶȡ�
        BITMAP          m_bm;                           //ԭʼλͼ��Ϣ
        BYTE          * m_pBoardBitmapBits;             //ָ��λͼ���ݿ飨��ʼ�������ݣ�

		//��������Ҳ���Ա�����Ϊ��һ��Ѱ�����̴��ڵİ취
        int             m_MaxParentWinCountBoard;       //ͼ�����̴����丸���ڼ���
        char            m_ClassNameBoard[10][254];      //ͼ�����̴����丸��������
        char            m_WinNameBoard[10][254];        //ͼ�����̴����丸���ڱ���

		//�������ݿ��Ը��ݴ��ھ���ȼ����������˿ɲ����ڷ�����
        POINT           m_iLeftTop;  //�����Ƕ���ı�����δ��ϸ��顣������������ָ��ڳ���λ�ã������Ļ��
        POINT           m_LRookInBoard;                 //m_LRookInBoard�ڳ����ĵ���������̵�λ�ã��൱������ԭ�㣬to Board client

        HBITMAP         m_hbitmap;                      //��ǰ����λͼ���

		HWND            m_hWndProg;                     //��ǰ�ͻ��˳��򴰿ھ�������㣩
        HWND            m_hWndBoard;                    //��ǰ�ͻ������̴��ھ������������
        RECT            m_ProgWinRect  ;                //top window rect
        RECT            m_BoardWinRect  ;               //board  rect

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CUcci&          m_ucci;                         //Ӧ������ΪUCCI����ָ�룬���ݲ�ͬ�������ʼ���Խ�ʡ�ռ䡣���޸�

        BYTE            m_position[10][9];              //��ǰ������������
        bool            m_bQiShouRed;                   //�Ƿ��ú��壿
		int				m_iChessMoveFrom;
        bool            m_bQiShouBottom;                //if I am in bottom in  NewQiJu
        CHESSMOVE       m_Move;							//��ǰ�����߲�

		//���������ӵ�����
		double	m_threshold;		//���ƶ���ֵ
		cv::Mat m_imgBoard;			//ʵʱ����ͼ��
		cv::Mat m_imgBoardInit;		//��ʼ����ͼ��
		cv::Mat m_imgPiece[15];		//��ʼ���̵����ĸ�����ͼ�񣬱��Ϊ1��14��B_KING��1 R_PAWN=14; 0Ϊ�� ��m_imgBoardInitΪǳ����
private:
        bool            GetFromFile(char * filename);
        
		CHESSMOVE		Position2Move(BYTE pre_position[10][9] );    //ͨ���Ƚ�����positionȡ���岽
        bool			ValidPosition(BYTE position[10][9]);
		
		//�������ݲ���
		char            m_cStepBuff[20];
        WCHAR           m_wsQiShouMove[8];
        long            m_dx;  //���̸��һ������¾�����䣬���������
        long            m_dy;  //���̸�ߣ�һ������¾�����䣬��������ȣ����������Ҫ���Ǻӽ�Ĳ���
        BYTE          * m_pBMPQiziBits ;        //���������ӵ�λͼ���ݣ���ʼ�������ݣ�
        BYTE          * m_pCurBMPBoardBits;     //ʵʱ����λͼ����
};

//ϵͳtools(ȫ�ֺ���)
CHESSMOVE Str2Move(const char str[5]);
void ErrorHappen(LPTSTR lpszFunction);
void StrToClip(char* pstr); //����FEN����������

extern const BYTE	InitChessBoard[10][9];		//��ʼ���̶���
extern const POINT  m_InitPosOfPieceType[15];	//14��������ʼλ�ã������ߣ�
