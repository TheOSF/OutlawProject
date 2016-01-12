#pragma once 

#include "../Render/MeshRenderer.h"
#include "../GameSystem/GameObject.h"
#include "../Camera/Camera.h"
#include "Item_WallBoundBall.h"
#include "../character/CharacterBase.h"
#include <vector>

//-------------------------------------------------------------//
// アイテム？　ベル
//-------------------------------------------------------------//

class Item_Bell : public GameObjectBase
{
public:
    static const Vector3 BellFreezePos;

    Item_Bell(UINT HitCount);
    ~Item_Bell();

    void Hit(Item_WallBoundBall* pHitBall,CharacterBase* pHitOwner);
    Vector3 GetPos();

    bool Update();
    bool Msg(MsgType mt);

private:
    typedef std::vector<Item_WallBoundBall*> WallBoundBallArray;

    WallBoundBallArray m_WallBoundBallArray;
    CharacterBase*    m_pLastHitChr;
    CameraDrawObject  m_CameraDrawObject;
    void(Item_Bell::* m_pStateFunc)();
    MeshRenderer*     m_pMeshRenderer;
    Vector3           m_Pos;
    const int         m_MaxHitCount;
    int               m_HitCount;
    int               m_NoDamageFrame;

    void UpdateBallLive();
    void MeshUpdate();

    void State_MoveToTarget();
    void State_WaitHit();
    void State_ApperSkillSoul();
    void State_FadeOut();
    void State_Finish();
};