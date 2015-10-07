#pragma once

#include "../GameSystem/GameObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/MeshRenderer.h"

//----------------------------------------------------------//
// 物理シュミレーションで動かすステージオブジェクトクラス
//----------------------------------------------------------//

class StagePhysicMoveObject :public GameObjectBase
{
public:

    StagePhysicMoveObject(
        MeshRenderer* pRenderMesh,       //描画するメッシュ
        RigidBody*    pRigidBody,        //描画
        const Matrix& InitTransMatrix,   //初期姿勢
        UINT          FadeOutStartFrame, //フェードアウトし始めるフレーム
        UINT          FadeOutEndFrame    //フェードアウトして消えるフレーム
        );

    ~StagePhysicMoveObject();


private:

    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};