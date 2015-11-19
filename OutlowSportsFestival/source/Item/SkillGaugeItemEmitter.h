#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"


//---------------------------------------------------------//
//  �X�L���Q�[�W�񕜃A�C�e�����o��������N���X
//---------------------------------------------------------//

class SkillGaugeItemEmitter :public GameObjectBase
{
public:

    SkillGaugeItemEmitter(int ApperAvrFrame);
    ~SkillGaugeItemEmitter();

private:
    const int  m_ApperAvrFrame;
    bool       m_isEmmite;
    bool       m_LoseEmitte;
    int        m_EmitteCount;

    bool CheckLosePlayer(CharacterBase** ppOut);
    void SetApperFrame();

    bool Update();
    bool Msg(MsgType mt);
};