#pragma once

#include "../GameSystem/GameObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/MeshRenderer.h"
#include "../Damage/Damage.h"

//----------------------------------------------------------//
// 物理シュミレーションで動かすステージオブジェクトクラス
//----------------------------------------------------------//

class StagePhysicMoveObject :public GameObjectBase
{
public:

    StagePhysicMoveObject(
        MeshRenderer*       pRenderMesh,
        RigidBody*          pRigidBody,
        CrVector3           Scale,
        float               HitScale
        );

    ~StagePhysicMoveObject();

    bool  m_Destroy;

private:

    MeshRenderer*       m_pRenderMesh;
    RigidBody*          m_pRigidBody;
    const Vector3       m_Scale;
    const float         m_HitScale;
    int                 m_HitCount;

    void MeshUpdate();
    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};