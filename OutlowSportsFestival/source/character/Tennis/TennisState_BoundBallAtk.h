#pragma once

#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"

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
        CrVector3  pos,         //初期座標
        CrVector3  first_move,  //１回目の移動の移動量
        TennisPlayer* const pOwner   //発射主へのポインタ
        );

    ~TennisBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    typedef void(TennisBoundBall::*StateFunc)();
    
    StateFunc      m_pStateFunc;    //現在の更新を行う関数
    int            m_Timer;         //作業用タイマー

    TennisPlayer*const  m_pOwnerTennis;  //親テニス
    MeshRenderer*       m_pBallRenderer; //ボール描画クラス
    Locus               m_Locus;         //軌跡クラス
    RigidBody*          m_pRigidBody;    //このボールを動かす剛体クラス
    Matrix              m_BaseMatrix;    //スケールを除いた変換行列
    DamageShpere        m_Damage;        //ダメージ判定クラス

    void StateFly();         //放物線運度中のステート
    void StateGroundTouch(); //地面に着弾した瞬間のステート
    void StateMove();        //等速直線運動中のステート
    void StateNoDamage();    //攻撃終了後の後は消えるだけステート
    void StateFinish();      //フィニッシュ(存在自体が消える)ステート

    void StateCreateFadeOutBall(); //フェードアウトして消えるボールを作る

    void CreateRigidBody();  //剛体生成
    void AddLocusPoint();    //軌跡のポイントを現在のパラメータで一点追加する
    void UpdateMesh();       //メッシュ更新
    void UpdateLocusColor(); //軌跡色更新
    void UpdateDamage();     //当たり判定の更新

    //ステート関数の切り替え
    void SetState(StateFunc pNextState);

    //カウンターをした際の処理
    void Counter(CharacterBase* pCounterCharacter)override;
};
