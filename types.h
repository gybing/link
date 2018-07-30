//PieceType
#define NOCHESS 0    //û������
#define B_KING       1  //��˧
#define B_CAR        2  //�ڳ�
#define B_HORSE      3  //����
#define B_CANON      4  //����
#define B_BISHOP     5  //��ʿ
#define B_ELEPHANT   6  //����
#define B_PAWN       7  //����
#define R_KING       8   //��˧
#define R_CAR        9   //�쳵
#define R_HORSE      10  //����
#define R_CANON      11  //����
#define R_BISHOP     12  //��ʿ
#define R_ELEPHANT   13  //����
#define R_PAWN       14  //���
#define B_BEGIN      B_KING
#define B_END        B_PAWN

//PieceID
#define B_PAWN1       1  //����
#define B_PAWN2       2  //����
#define B_PAWN3       3  //����
#define B_PAWN4       4 //����
#define B_PAWN5       5 //����
#define B_ELEPHANT1   6  //����
#define B_ELEPHANT2   7  //����
#define B_BISHOP1     8  //��ʿ
#define B_BISHOP2     9  //��ʿ
#define B_CANON1      10  //����
#define B_CANON2      11  //����
#define B_HORSE1      12 //����
#define B_HORSE2      13  //����
#define B_CAR1        14 //�ڳ�
#define B_CAR2        15  //�ڳ�
#define B_KING1		  16  //��˧
#define R_BEGIN      R_KING
#define R_END        R_PAWN

#define R_PAWN1       17  //����
#define R_PAWN2       18  //����
#define R_PAWN3       19  //����
#define R_PAWN4       20  //����
#define R_PAWN5       21  //����
#define R_ELEPHANT1   22  //����
#define R_ELEPHANT2   23  //����
#define R_BISHOP1     24  //��ʿ
#define R_BISHOP2     25  //��ʿ
#define R_CANON1      26  //����
#define R_CANON2      27  //����
#define R_HORSE1      28  //����
#define R_HORSE2      29  //����
#define R_CAR1        30  //�쳵
#define R_CAR2        31  //�쳵
#define R_KING1       32  //��˧

#define ISBLACK(x)  (x>=B_BEGIN&&x<=B_END)
#define ISRED(x)        (x>=R_BEGIN&&x<=R_END)
#define ISSAMESIDE(x,y)  ((ISBLACK(x)&&ISBLACK(y))||(ISRED(x)&&ISRED(y)))

//
//typedef struct _chessmove
//{
//	short	  	ChessID;
//	POINT	From;
//	POINT	To;
//	int             Score;
//    short           KillID;
//}CHESSMOVE;

//        �������ϣ�ԭʼ״̬�£�����2��3��ʾ���£�
//        CHESSMOVE mv;
//        mv.ChessID=B_HORSE;
//        mv.From.x=1;
//        mv.From.y=0;
//        mv.To.x=2;
//        mv.To.y=2;
