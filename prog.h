//---------------------------------------------------------------------------
#ifndef progH
#define progH

#include "ucci.h"

typedef unsigned char      BYTE;
typedef int                BOOL;
#define FALSE                0
#define TRUE                 1
#define NOCHESS 0    //û������
#define B_KING       1  //��˧
#define B_CAR        2  //�ڳ�
#define B_HORSE      3  //����
#define B_CANON      4  //����
#define B_BISHOP     5  //��ʿ
#define B_ELEPHANT   6  //����
#define B_PAWN       7  //����
#define B_BEGIN      B_KING
#define B_END        B_PAWN
#define R_KING       8   //��˧
#define R_CAR        9   //�쳵
#define R_HORSE      10  //����
#define R_CANON      11  //����
#define R_BISHOP     12  //��ʿ
#define R_ELEPHANT   13  //����
#define R_PAWN       14  //���
#define R_BEGIN      R_KING
#define R_END        R_PAWN
#define IsBlack(x)  (x>=B_BEGIN&&x<=B_END)
#define IsRed(x)  (x>=R_BEGIN&&x<=R_END)
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

typedef struct _chessmanposition
{
	short x;
	short y;
}CHESSMANPOS;

typedef struct _chessmove
{
	short	  	ChessID;
	CHESSMANPOS	From;
	CHESSMANPOS	To;
	int               Score;
        short           KillID;
}CHESSMOVE;

class Cqishou
{
  public:
        char            m_ptcName[50];
        int             m_iChessMoveFrom;               //�������ͣ��߲���Դ����0-���棻1��2-�����������л�ȡ�����ֻ�ͼ�񣩣�3��4-���루���̻�������������5�����������;��
        bool            m_bProgBottom;                  //if computer above prog
        bool            m_bQiShouRed;                   // if red
        bool            m_bQiShouBottom;                //if i am bottom in  NewQiJu
        bool            m_bUseOpenBook;                 //�Ƿ�ʹ�ÿ��ֿ�
        bool            m_forbid;                         //�Ƿ񷸹� ������ѭ������
        bool            m_bookfind;                       // if find move in openbook?

        CUcci           m_ucci;

        HWND            m_hWndProg;
        HWND            m_hWndBoard;                    //
        HWND            m_hWndSteps;                    // window handle of listbox steps;

        BITMAP          m_bm;
        HBITMAP         m_hbitmap;
        BYTE            * m_pBMPData;

        int             m_MaxParentWinCountBoard;
        char            m_ClassNameBoard[10][254];
        char            m_WinNameBoard[10][254];
        int             m_MaxParentWinCountSteps;
        char            m_ClassNameSteps[10][254];
        char            m_WinNameSteps[10][254];

        POINT           m_iLeftTop;                     //to Screnn
        POINT           m_OXY;                          //to Board client
        POINT           m_iPointInBoard ;               //to PROG Client
        POINT           m_iPointInSteps ;               //to Steps Client
        RECT            m_ProgWinRect  ;                //topwindow rect
        RECT            m_BoardWinRect  ;               //board rect
        RECT            m_StepsWinRect  ;               //board rect

        BYTE            m_dx;
        BYTE            m_dy;
        BYTE            m_iDI;                          //�ֱ���
        unsigned long int             m_iDD;                          //���
        BYTE            m_position[10][9];
        long int        m_scores;                         //�÷�
        CHESSMOVE       m_cmQiShouStep;
        CHESSMOVE       m_cmMyHistroyMove[6];           //�ҵ���ʷ�߲�

        int             m_iFirstTime;
        int             m_iFirstSteps;                  //��һʱ�ޣ��� m_iFirstTime���ӣ���������m_iFirstSteps��
        int             m_iSecondTime;
        int             m_iSecondSteps;                 //�ڶ�ʱ�ޣ���m_iSecondTime������m_iSecondStemps��
        int             m_iTotalTimes;                  //����ʱ

        HWND            FindWinBoardAndSteps(int maxparent, char winclass[10][254],char winname[10][254]);
        bool            Go(CHESSMOVE cmQiJuMove, bool bBottomGo);

        bool            CalcDx();
        bool            Similiar(POINT DestPt, POINT SrcPt );
        HBITMAP         GetBMPBoard();
        bool            LocateBlackCar(POINT DestPt, POINT SrcPt );
        bool            SaveBoardData(HBITMAP hbitmap);
        BYTE *          BMP2ChessBoard(HBITMAP hbitmap);

        Cqishou();
        ~Cqishou();

};

class  Cnewqiju
{
        public:
	        BYTE            m_ChessBoard[10][9];
                bool            m_bBottomGo;
                CHESSMOVE       m_cmQiJuMove;
                CHESSMOVE       m_cmQiPu[200];                  //��������    �200��
                WCHAR           m_wsQiJuMove[8];                //

                WCHAR           m_wsBestMove[5];                //��ǰ����߲����й�ʽ��ʾ��
                WCHAR           m_wsBetterMove[5];              //��ǰ�κ��߲����й�ʽ��ʾ��
                WCHAR           m_wsGoodMove[5];                //��ǰ���߲����й�ʽ��ʾ��

                char            m_cStepBuff[20];
	        int             m_iTurn;
                bool            m_bGameOver;
                bool            m_bConnected;

                Cqishou         m_PlayerBottom;
                Cqishou         m_PlayerTop;
                Cqishou *       m_WhichPlayer;
                struct {
                        int year;
                        int month;
                        int day;
                        }  m_asChessDate ;              //��������
                struct {
                        int hour;
                        int minitue;
                        int second;
                        }  m_asChessTime;               //����ʱ��
                char m_asChessaddress[];                //�����ص�
                char m_asChessName[];                   //��������
                BYTE result;                            //�������


                void            WCHAR2CHESSMOVE(WCHAR wsChessMove[5], CHESSMOVE &cmChessMove );
                void            CHESSMOVE2WCHAR(CHESSMOVE cmChessMove , WCHAR wsChessMove[5]);
                BYTE            MakeMove(CHESSMOVE* move) ;
                void            UnMakeMove(CHESSMOVE* move,BYTE nChessID)  ;
                void            DrawCchessBorder(Cqishou player);
                void            DrawStepsBorder(Cqishou player) ;

                BOOL            GetQiShouMove(Cqishou *player);
                char*            MyGetListViewItem(Cqishou * player)  ;
	        BOOL 	        IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);
        	bool            SaveToFile(char * filename);
        	bool            GetFromFile(char * filename);
                void            Start();
                void            ContiSame(Cqishou playerto , Cqishou playerfrom );
                int             IsGameOver(BYTE position[10][9]);
                void            ChangeSide(BYTE position[10][9]);
                BYTE            GetBMPMove(Cqishou *player);

                char *           CopyJuMian(BYTE position[10][9] , bool RedGo );
                BYTE *           PasteJuMian(char FenStr[2054]);

//                CSearchEngine   *m_pSE;
		BYTE            computerthink();		        //���ݵ�ǰ����һ����

	        Cnewqiju();
                ~Cnewqiju();
};

const BYTE InitChessBoard[10][9]=
{
    {
        B_CAR,B_HORSE,B_ELEPHANT,B_BISHOP,B_KING,B_BISHOP,B_ELEPHANT,B_HORSE,B_CAR
    }
    ,
    {
        NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS
    }
    ,
    {
        NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS
    }
    ,
    {
        B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN
    }
    ,
    {
        NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS
    }
    ,

    {
        NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS
    }
    ,
    {
        R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN
    }
    ,
    {
        NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS
    }
    ,
    {
        NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS
    }
    ,
    {
        R_CAR,R_HORSE,R_ELEPHANT,R_BISHOP,R_KING,R_BISHOP,R_ELEPHANT,R_HORSE,R_CAR
    }
}
;




//---------------------------------------------------------------------------
#endif
