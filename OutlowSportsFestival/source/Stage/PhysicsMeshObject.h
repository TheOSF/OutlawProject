#pragma once

#include "iextreme.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../GameSystem/GameObject.h"

//----------------------------------------------------------------------------
//　　　　 物理計算でメッシュレンダラーを動かすオブジェクトクラス
//----------------------------------------------------------------------------

class PhysicsMeshObject :public GameObjectBase
{
public:
    PhysicsMeshObject(
        MeshRenderer* pRenderer,
        RigidBody*    pRigitBody,
        UINT          DeleteTime
        );

    ~PhysicsMeshObject();

private:

    MeshRenderer* const m_pRenderer;
    RigidBody*    const m_pRigitBody;
    UINT                m_DeleteTime;
    Matrix              m_BaseMatrix;
    
    bool Update();
    bool Msg(MsgType mt);
};