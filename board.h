/*************************************************************************************************************************
������   : CXXX
����     : <��Ҫ˵����������ɵĹ���>
�쳣��   : <���ڸ�����쳣�ࣨ����еĻ���> 
--------------------------------------------------------------------------------------------------------------------------
��ע     : <ʹ�ø���ʱ��Ҫע������⣨����еĻ���> 
�����÷� : <��������ʹ�÷����ϸ��ӻ����⣬�������͵Ĵ�������> 
--------------------------------------------------------------------------------------------------------------------------
����     : <xxx>
*************************************************************************************************************************/

#pragma once

#include "bmpPlayer.h"

class  CNewQiJu
{
public:
        CNewQiJu();
        ~CNewQiJu();
        BYTE            MakeMove(CHESSMOVE* move) ;
        void            UnMakeMove(CHESSMOVE* move,BYTE nChessID)  ;
        void            Start();
        int             IsGameOver(BYTE position[10][9]);
        BYTE *          PasteJuMian(char FenStr[2054]);
        void            WCHAR2CHESSMOVE(WCHAR wsChessMove[5], CHESSMOVE &cmChessMove );

		
        static void     ChangeSide(BYTE position[10][9]);
        static BOOL     IsValidMove(BYTE position[10][9], CHESSMOVE mv);

public:
        BYTE            m_ChessBoard[10][9];
        bool            m_bBottomGo;
        CHESSMOVE       m_cmQiJuMove;
        bool            m_bConnected;

        CUcci           m_PlayerUcci;                //��������ں����������ֶ���ǰ�涨��
        CMatPlayer      m_PlayerBottom;
        CMatPlayer      m_PlayerTop;
        CMatPlayer *    m_WhichPlayer;
private:
        int             m_iTurn;
        bool            m_bGameOver;
};