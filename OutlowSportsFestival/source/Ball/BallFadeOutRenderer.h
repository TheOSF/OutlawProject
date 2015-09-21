#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../Library/Bullet/BulletSystem.h"

//--------------------------------------------------
//  フェードアウトで消えるボールクラス
//--------------------------------------------------

class BallFadeOutRenderer :
    public GameObjectBase,
    public ForwardRenderer
{
public:

    BallFadeOutRenderer(
        LPIEXMESH     pBallMesh,       //ボールのメッシュへのポインタ(deleteしない)
        const Matrix& BaseTransMatrix, //スケール、回転行列
        RigidBody*    pRigidBody,      //剛体クラスへのポインタ
        UINT          delete_frame     //消えるまでのフレーム
        );

    ~BallFadeOutRenderer();


private:
    LPIEXMESH        m_pBallMesh;       //ボールのメッシュへのポインタ(deleteしない)
    const Matrix     m_BaseTransMatrix; //スケール、回転行列
    RigidBody* const m_pRigidBody;      //剛体クラスへのポインタ
    const UINT       m_Delete_frame;    //消えるまでのフレーム
    UINT             m_Counter;         
    Vector3          m_Pos;

    bool Update();
    bool Msg(MsgType mt);

    void Render();
    void CalcZ();

};