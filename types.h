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
#define IsRed(x)        (x>=R_BEGIN&&x<=R_END)
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

#define ENGINE_PLAYER           0  //�岽������UCCI����
#define BMP_PLAYER              1  //�岽������ͼ�οͻ���
#define KEYBOARD_PLAYER         2  //�岽������������
#define EDIT_PLAYER             3  //�岽�������ı��༭���������
#define INTERTNET_PLAYER        4  //�岽����������
#define VOICE_PLAYER            5  //�岽����������ʶ��

#define MAX_PLAYER_NAME         50 //���ڱ��������������󳤶�
#define THRESHOLD               100//���ƶ���ֵ
#define COLORBITSCOUNT          8  //��ɫλ��

typedef struct _chessmanposition
{
	long x;
	long y;
}CHESSMANPOS;    //Լ���������Ͻ�Ϊ��0,0��
//        �������ϣ�ԭʼ״̬�£�����2��3��ʾ���£�
//        CHESSMOVE mv;
//        mv.ChessID=B_HORSE;
//        mv.From.x=1;
//        mv.From.y=0;
//        mv.To.x=2;
//        mv.To.y=2;

typedef struct _chessmove
{
	short	  	ChessID;
	CHESSMANPOS	From;
	CHESSMANPOS	To;
	int             Score;
    short           KillID;
}CHESSMOVE;
