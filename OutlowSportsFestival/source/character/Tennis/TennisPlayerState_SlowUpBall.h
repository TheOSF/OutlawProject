#pragma once


#include "../../Ball/Ball.h"
#include "../../utillity/Locus.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../GameSystem/GameObject.h"
#include "../../Damage/Damage.h"

//*****************************************************
//		投げ上げるボールクラス
//*****************************************************

class TennisUpBall :public GameObjectBase, public BallBase
{
public:

    //コンストラクタ
    TennisUpBall(
        BallBase::Params	params			//ボールパラメータ
        );

    ~TennisUpBall();


    bool Update();
    bool Msg(MsgType mt);

private:

    bool(TennisUpBall::*m_pStateFunc)();
    LpMeshRenderer		m_pMeshRenderer;
    DamageCapsure		m_Damage;
    int                 m_DeleteFrame;
    Locus               m_Locus;
    RigidBody*          m_pRigitBody;
    Vector3             m_RotateSpeed;

    bool isOutofField()const;  //フィールド外に出ているか

    void UpdateDamageClass();  //ダメージ判定の位置を現在の位置に更新
    void UpdateLocusColor();   //軌跡の色を現在の親キャラクタの色に設定
    bool UpdateWallCheck(Vector3& outNewMove);    //壁との判定を取り、接触していたなら移動値を反射してステート移行をする
    void AddLocusPoint();      //軌跡のポイントを現在のパラメータで一点追加する

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();           //攻撃判定のない状態にする

    bool StateFlyMove();
    bool StatePhysicMove();
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

