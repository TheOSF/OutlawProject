#pragma once 

#include "../Render/MeshRenderer.h"
#include "../GameSystem/GameObject.h"

//-------------------------------------------------------------//
// アイテム？　ベル
//-------------------------------------------------------------//

class Item_Bell : public GameObjectBase
{
public:
    static const Vector3 BellFreezePos;

    Item_Bell(UINT HitCount);
    ~Item_Bell();

    bool Update();
    bool Msg(MsgType mt);

private:
    void(Item_Bell::* m_pStateFunc)();
    MeshRenderer*     m_pMeshRenderer;
    Vector3           m_Pos;
    const int         m_MaxHitCount;
    int               m_HitCount;
    int               m_NoDamageFrame;

    void MeshUpdate();
    bool HitCheck(Vector3& out_hitPos);

    void State_MoveToTarget();
    void State_WaitHit();
    void State_ApperSkillSoul();
    void State_FadeOut();
    void State_Finish();
};