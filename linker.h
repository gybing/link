/*******************************************************************************
������  : CMatLinker
-------------------------------------------------------------------------------
����    : ���ݷ�����������ͼ�����fen����
		   1�������˹���λ�ڷ����Ϸ�����λ�õķ�ʽ����ͼ�����߷����������ɴ�ȡ��
		   2�����÷�����λͼ�οͻ������̴��ھ������ȡ����ͼ��
		   3������ʵʱͼ��ʶ�𲢷���fen����
		   4���ڿͻ���ģ����궯���Զ�����
		   5�������õ�3�����棨����ӽ��棩�������ģʽ��Э���˻�
--------------------------------------------------------------------------------
�����÷� : 	
		//��һ��(�൱��ͼ��Ԥ����ͳһ���ŵ�һ����С)
		//ʵʱ������������
*******************************************************************************/
#pragma once

#include "tools.h"
#include "..\\jqxq\\types.h"
#include "..\\jqxq\\piece.h"
#include "..\\jqxq\\square.h"

typedef struct {				
	string	titleKeyword;		//���̿ͻ��˳��򴰿ڱ��⣨��ؼ��֣������ڲ��ҿͻ��˳��򴰿ھ��������ԭʼ����ͼ�ļ���fnInit.jpg
	POINT	ptBRookInProg;		//�����ڿͻ��˳��򴰿��е�λ�ã����Ͻ�Ϊ��0��0������������Ͽͻ��˳��򴰿ھ���������̴��ھ��������һ�ַ�����ͨ��ʵʱ��������ʱȡ�õĴ�����Ϣ�������������̴��ھ����
	POINT	ptBRookInBoard;		//����������ͼ�е�λ�ã����Ͻ�Ϊ��0��0����ptBoard[0][0]��ֵ��ͬ���������ԭʼ����ͼ�ļ�ȷ�����̸������ǷǱ�׼���̣�����ptBoard[10][9]��ȷ�����������λ��
	int		widthInit;			//ԭʼ����ģ��ֱ��ʣ����ڻ�ȡԭʼ����ģ��ͼ����
	int		widthReal;			//ԭʼ����ģ��ֱ��ʣ����ڻ�ȡԭʼ����ģ��ͼ����		�������±������	//int		delt;			//widthInit-delt=ʵʱ���ӷֱ��ʣ����ڱ�׼����ͼΪ0
	double	threshold;			//��ֵ
	Mat*	pieces;				//ԭʼ����ģ������
	//�������ݷ����зǱ��裬���������
	POINT	ptBoard[10][9];		//����������ͼ�е�λ�ã����ڶԸ����������̣���һ�㲻��Ҫ�����һ���ڽ�ȡ����ͼʱ��deltY=y;deltX=x/2��
	int     maxParents;			//
	vector <string> className;	//ͼ�����̴��ڼ��丸����������������С��ͼ�����̴��ڼ��丸���ڼ�������˿�ʡ�Ե������������
	vector <string> titleName;	//ͼ�����̴��ڼ��丸���ڱ���

	void write_solution(void);
	void read_solution(void);
} SOLUTION;						//��������

class CMatLinker
{
  public:
        CMatLinker(string config="CycloneGui �Ż��ߣ�����Ҧ.ini", BYTE width=35, BYTE width_real=30, double f=0.035);
        ~CMatLinker();

		bool			isready(void) { return (((m_flag=="91069") && find_win_prog()&&find_win_board())? true:false); };		//���ط���������־
		bool			make_solution(POINT MousePoint);				//ȡ�ô�����Ϣ�����������������淽��
        bool            load_solution(string filename);								//�ҵ������ڼ�������������
		bool			make_move(POINT mvFrom, POINT mvTo)const;		//ģ������������Զ�����
		void			update_fen(void);								
		const string	mat2fen(string fn, int match_method = 1);		//ʶ������ͼ�ļ���CV_TM_SQDIFF_NORMED=1
		Color			local_color(void);								//�ҷ�(�·�)������ɫ
		void			ChangeSide(void);								//��ת����180��
		const string	print(void) const;								//���С����ͼ��Ϣ
		string			uci_pv(std::vector<Move> pv);								//�������PV
		Move			board2move(PieceID pre_board[SQUARE_NB]);		//ͨ���Ƚ�����positionȡ���岽
		int				board2diffs(PieceID pre_board[SQUARE_NB]);		//ͨ���Ƚ�����positionȡ��λ�ò�ͬ�ĵ������
		void			set(const std::string& fenStr );				
		const string	fen( ) const ;									
		const string	GetStepName( Move move )const;		
		void			do_move(Move move);

		void			set_threshold(double threshold)		{ m_threshold = threshold; };
		void     		set_width_init(int width_init)		{ m_width_init = width_init; };
		void			set_width_real(int width_real)		{ m_width_real = width_real; };
		double			get_threshold(void)					{ return m_threshold;};
		int     		get_width_init(void)				{ return m_width_init;};
		int				get_width_real(void)				{ return m_width_real;};
		PieceID			piece_on(Square s) const			{ return m_Board[s];};
		bool			empty(Square s) const				{ return piece_on(s)==NOCHESS;};

		PieceID 		m_Board[SQUARE_NB];					//��ǰ���棬��ֵΪ�����������ݣ�Ҳ��Ϊ�������ݣ���Լ�������º���
		Color			sideToMove;							//�ֵ��ķ������ˣ�
		string			fenold;
		POINT			from, to;							//���ӵ�λ��

private:
		bool			build_solution(POINT pt, string fn);		//��ԭʼ����ͼ�񴴽�����
		bool			build_solution(POINT pt);					//�����ȼ�ʱ, ����������ʵʱλ�ô�������	
		bool            write_solution(string filename);			//
        bool            read_solution(string filename);				//
		const string	mat2fen(int match_method = 1);				//ʵʱʶ���ͼ

		bool			pieces_all(int type =32);					//��ԭʼ������ͼ�зָ��32������ͼ��
		bool			calc_DxDy(void);							//Ѱ���Һڳ�������m_dx
		bool			normalize(char* fn, cv::Point pt, int width = 40, int delt=6, double fLookRook=0.9);
		bool			normalize(char* fn);						//��һ��(�൱��ͼ��Ԥ����ͳһ���ŵ�һ����С)
		void			get_mat_real();								//�����ͻ������̴���
        HWND            find_win_prog(void) ;						//Ѱ�ҳ��򴰿�
        HWND            find_win_board(void) ;						//Ѱ�ҳ������̴���
		cv::Mat&		fen2mat(string strfen, Mat& dst);			//��fen��ת�������Ӧ��Mat
		void			LeftToRight(void);							//���ұ任

		//����˽�����ݲ��֣�
		//ʵʱ�仯������
		cv::Mat			m_imgReal;						//��ʶ���ʵʱ����ͼ��
        
		//�����ļ���־�淽��������־ if(m_flag=="91069")��־����׼����
		string          m_flag;							

		//ͼ�οͻ��˷��������ٰ����������ݣ���SOLUTION�Ķ��壩
        string          m_titleKeyword;					//���ڱ���ؼ���
		POINT           m_ptBRookInBoard;               //�ڳ�����ڿͻ��˳����е����̴���λ�ã��൱������ԭ�㣬to m_hWndBoard client
		POINT			m_ptBRookInProg;				//�ڳ�����ڿͻ��˳��򴰿�λ�ã��൱������ԭ�㣬to m_hWndProg client
        int				m_width_init;					//�ֱ��ʡ�ԭʼ�������ӿ�=��  ���ڽ�������ʱ
		int				m_width_real;					//�ֱ��ʡ�ʵʱ���ӿ�=��  ����ʶ��ʱ��ȡʵʱ����  m_width_init>=m_width_real
		double			m_threshold;					//���ƶ���ֵ
		//POINT			ptBoard[10][9];					//����������ͼ�е�λ�ã����ڶԸ����������̣���һ�㲻��Ҫ�����һ���ڽ�ȡ����ͼʱ��deltY=y;deltX=x/2��
		//��������Ҳ���Ա�����Ϊ��һ��Ѱ�����̴��ڵİ취
        int             m_maxParents;					//ͼ�����̴����丸���ڼ���
        char            m_className[10][254];			//ͼ�����̴����丸��������
        char            m_titleName[10][254];			//ͼ�����̴����丸���ڱ���
		//SOLUTION        m_solution;					//�����ǵ�ǰ���������ṹ�ɴ�һ���Ϸ��㣬�������write_solution �� read_solution���ܡ�
		
		//�������ݿɴӷ����еó�
		HWND            m_hWndProg;                     //��ǰ�ͻ��˳��򴰿ھ�������㣩
        HWND            m_hWndBoard;                    //��ǰ�ͻ������̴��ھ������������
        long            m_dx;							//���̸��һ������¾�����䣬��������ȣ��������
        long            m_dy;							//���̸�ߣ�һ������¾�����䣬��������ȣ����������Ҫ���Ǻӽ�Ĳ���
		cv::Mat			m_imgInit;						//��ʼ����ͼ��, �����п��Խ���������ģ�����ݣ��ɼ����ļ���С����������ʱû�������̷��㡣
		cv::Mat			m_imgPieceAll;					//��ʼ���̵����ĸ�����ͼ��ϲ�
};

extern CMatLinker link;