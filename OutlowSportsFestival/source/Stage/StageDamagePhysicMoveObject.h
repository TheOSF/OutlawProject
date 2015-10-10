#pragma once

#include "../GameSystem/GameObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/MeshRenderer.h"
#include "../Collision/MeshCollider.h"

//-------------------------------------------------------------------------------//
// ダメージを受けると物理シュミレーションで動かすステージオブジェクトクラス
//-------------------------------------------------------------------------------//

class StageDamagePhysicMoveObject :public GameObjectBase
{
public:

    class RigidBodyCreater
    {
    public:
        virtual ~RigidBodyCreater(){}
        virtual RigidBody* Create(CrVector3 v) = 0;
    };

    StageDamagePhysicMoveObject(
        MeshRenderer* pRenderMesh,       //描画するメッシュ
        CrVector3     pos,
        CrVector3     scale,
        CrVector3     angle,
        float         HitScale,           //あたり安定の大きさ
        RigidBodyCreater* pCreater,
        MeshCollider*     pCollider,      //当たり判定メッシュ(ダメージがあたるとdeleteする)
        CrVector3     diff_pos = Vector3Zero
        );

    ~StageDamagePhysicMoveObject();

private:
    RigidBodyCreater*   m_pCreater;
    MeshRenderer*       m_pRenderMesh;
    const float         m_HitScale;
    const Vector3       m_Pos, m_Scale, m_Angle, m_Diffpos;
    MeshCollider* const m_pCollider;
    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};