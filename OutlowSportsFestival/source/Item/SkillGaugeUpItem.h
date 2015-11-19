#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../Render/MeshRenderer.h"
#include "../character/CharacterBase.h"

//---------------------------------------------------------//
//  スキルゲージ回復アイテム
//---------------------------------------------------------//

class SkillGaugeUpItem :public GameObjectBase
{
public:

    SkillGaugeUpItem(CrVector3 pos, CrVector3 move);
    ~SkillGaugeUpItem();
    
private:

    MeshRenderer*   m_pMeshRenderer;
    bool            m_LiveFlag;
    Vector3         m_Pos, m_Move, m_Power;

    void MeshUpdate();
    bool CalcNearCharacter(CharacterBase** ppOut)const;
    bool Update();
    bool Msg(MsgType mt);
};