#pragma once

//****************************************************
//	テニス_バウンドするボールクラス
//****************************************************

#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../utillity/LocusHDR.h"
#include "../../Render/LightObject.h"
#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"
#include "../../Ball/BallEffect.h"

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
    LocusHDR            m_Locus;         //軌跡クラス
    RigidBody*          m_pRigidBody;    //このボールを動かす剛体クラス
    Matrix              m_BaseMatrix;    //スケールを除いた変換行列
    DamageShpere        m_Damage;        //ダメージ判定クラス
    BallEffect          m_BallEffect;    //エフェクトクラス

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
