#pragma once

#include "../TennisPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	テニスプレイヤーの操作クラスヘッダー
//****************************************************


//プレイヤー操作の移動クラス
class TennisState_ComputerControll_Move :public TennisState
{
public:

    class MoveControllClass
    {
    public:
        virtual ~MoveControllClass(){}
        virtual Vector2 Move(TennisPlayer* t) = 0;
    };

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:
    CharacterUsualMove*		m_pMoveClass;
    MoveControllClass*      m_pMoveControllClass;
};


//サンドバッグコントロールクラス
class SandBagMoveControllClass :public TennisState_ComputerControll_Move::MoveControllClass
{
public:
    Vector2 Move(TennisPlayer* t)override;
};


class ComMoveControllClass :public TennisState_ComputerControll_Move::MoveControllClass
{
private:

    struct Param
    {
        RATIO   BallCounter;     //ボール対処率
        RATIO   BallCounterSpeed;//ボール反応速度
        RATIO   BallCounterTec;  //対処の的確さ
        RATIO   RunStop;         //立ち止まる率
        RATIO   RunPlaceTec;     //移動目標の正確さ
        RATIO   DangerEscape;    //危険察知の正確さ
        RATIO   ActionFrequence; //行動の頻度
        RATIO   NamePlay;        //なめプレイ度
    };

    Param m_Param;
    void(ComMoveControllClass::*m_pStateFunc)(Vector2&);

    int   m_Count;
    TennisPlayer* const m_pTennis;
public:

    ComMoveControllClass(TennisPlayer* t);
    ~ComMoveControllClass();

    Vector2 Move(TennisPlayer* t)override;

    void GetParams(Param& out, StrongType::Value st);

    void StateMove(Vector2& out);
    void StateStop(Vector2& out);

    void SwitchAction();
};
