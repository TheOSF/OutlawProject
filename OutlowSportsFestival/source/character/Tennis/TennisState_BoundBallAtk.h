#pragma once

#include "TennisPlayer.h"

//****************************************************
//	テニス_ボールバウンド攻撃クラス
//****************************************************

class TennisState_BoundBallAtk :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetBoundVec() = 0;
    };

    TennisState_BoundBallAtk(
        ControllClass* pControllClass   //バウンド方向をコントロールするクラス(終了時にdeleteする)
        );

    ~TennisState_BoundBallAtk();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:
    ControllClass* const m_pControllClass;
    int                  m_Timer;
};


//****************************************************
//	テニス_バウンドするボールクラス
//****************************************************
#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../utillity/Locus.h"
#include "../../Render/LightObject.h"

class TennisBoundBall :public GameObjectBase, public BallBase
{
public:

    //コンストラクタ
    TennisBoundBall(
        BallBase::Params	params,			//ボールパラメータ
        float               glavity,        //重力
        CrVector3           AtkVec          //バウンドする方向
        );

    ~TennisBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:

    LpMeshRenderer		m_pMeshRenderer;
    Locus               m_Locus;
    D3DXQUATERNION      m_BallRot;
    const Vector3       m_MoveVec;
    const float         m_Glavity;
    int                 m_Timer;
    void(TennisBoundBall::*m_pStateFunc)();

    void UpdateTransMatrix(RATIO scaleYratio);
    void GetTransMatrix(Matrix& m, RATIO scaleYratio);
    void RotateY();

    void State_Flying();
    void State_Failed();
    void State_Rolling();
    void State_AtkStart();
    void State_Finish(){}
};
