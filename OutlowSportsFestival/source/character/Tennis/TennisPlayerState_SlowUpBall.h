#pragma once


#include "../../Ball/Ball.h"
#include "../../utillity/Locus.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../GameSystem/GameObject.h"
#include "../../Damage/Damage.h"
#include "TennisPlayer.h"

//*****************************************************
//		投げ上げるボールクラス(ボール扱いではない)
//*****************************************************

class TennisUpBall :public GameObjectBase
{
public:

    //コンストラクタ
    TennisUpBall(
        CrVector3     pos,
        TennisPlayer* pTennis
        );

    ~TennisUpBall();

    bool Update();
    bool Msg(MsgType mt);

    Vector3 GetPos()const;

    void Destory();    //このボールを消去
    void DoRealTime(); //スローを無効にする

private:
    void(TennisUpBall::*m_pStateFunc)();
    TennisPlayer*           m_pTennis;
    MeshRenderer*           m_pMeshRenderer;
    RigidBody*              m_pRigidBody;
    Vector3                 m_Pos;
    float                   m_MoveY;
    int                     m_Timer;
    bool                    m_SlowFlag;
    
    void UsualMeshUpdate();

    bool isHitFloor()const;

    void State_SlowFly();
    void State_PhysicMove();
    void State_CreateFadeOut();
    void State_Destroy();
};




#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"

//****************************************************
//	テニス_ボールを上に上げるクラス
//****************************************************

class TennisState_SlowUpBall :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual void AngleControll(TennisPlayer* p) = 0;
        virtual bool SwitchState(TennisState_SlowUpBall* pState) = 0;
    };

    TennisState_SlowUpBall(
        ControllClass* pControllClass   //操作コントロールするクラス(終了時にdeleteする)
        );

    ~TennisState_SlowUpBall();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

    RATIO           GetChargePower()const;
    TennisUpBall*   GetSlowUpBall();
    void            DeleteSlowBall();

private:
    ControllClass* const m_pControllClass;
    TennisUpBall*        m_pUpBall;
    int                  m_Timer;
    const int            m_EndFrame;
};

