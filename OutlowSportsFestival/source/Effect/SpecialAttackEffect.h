#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterBase.h"
#include "../Render/LightObject.h"

//--------------------------------------------------------------------//
//  �X�L���������̃G�t�F�N�g�N���X
//--------------------------------------------------------------------//

class SpecialAttackEffect :public GameObjectBase
{
public:
    SpecialAttackEffect(CharacterBase* pParent, UINT Time);
    ~SpecialAttackEffect();

private:
    static const int MaxSpotLight = 3;
    CharacterBase* const  m_pParent;
    int                   m_Timer;
    const int             m_MaxTime;
    PointLight            m_Light;

    bool Update();
    bool Msg(MsgType mt);
};