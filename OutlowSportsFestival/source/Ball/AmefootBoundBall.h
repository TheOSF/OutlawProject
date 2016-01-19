#pragma once

#include "../GameSystem/GameObject.h"
#include "../Damage/Damage.h"
#include "../Ball/BallEffect.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../utillity/LocusHDR.h"
#include "Ball.h"


class AmefootPlayer;

// アメフトのバウンドするボールクラス
class AmefootBoundBall :public GameObjectBase
{
public:
    //コンストラクタ
    AmefootBoundBall(
        CrVector3  pos,         //初期座標
        CrVector3  first_move,  //１回目の移動の移動量
        AmefootPlayer* const pOwner   //発射主へのポインタ
        );

    ~AmefootBoundBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    typedef void(AmefootBoundBall::*StateFunc)();

    StateFunc           m_pStateFunc;    //現在の更新を行う関数
    int                 m_Timer;         //作業用タイマー
    Vector3             m_Move;
    Vector3             m_Pos;           
    RADIAN              m_ZRotate;

    AmefootPlayer*const m_pOwner;  //親キャラクタ
    MeshRenderer*       m_pBallRenderer; //ボール描画クラス
    LocusHDR            m_Locus;         //軌跡クラス

    void StateFly();         //放物線運度中のステート
    void StateGroundTouch(); //地面に着弾した瞬間のステート
    void StateFinish();      //フィニッシュ(存在自体が消える)ステート

    void AddLocusPoint();    //軌跡のポイントを現在のパラメータで一点追加する
    void UpdateMesh();       //メッシュ更新
    void UpdateLocusColor(); //軌跡色更新

                             
    void SetState(StateFunc pNextState); //ステート関数の切り替え
};


