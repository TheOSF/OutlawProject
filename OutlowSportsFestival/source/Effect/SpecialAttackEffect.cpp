#include "SpecialAttackEffect.h"
#include "../GameSystem/MatchLightManager.h"
#include "../Camera/CameraState.h"

SpecialAttackEffect::SpecialAttackEffect(CharacterBase* pParent, UINT Time) :
m_Timer(0),
m_MaxTime((int)Time),
m_pParent(pParent)
{
    m_Light.Visible = true;
    m_Light.param.color = Vector3(1, 0.5f, 0.1f);
    m_Light.param.pos = m_pParent->m_Params.pos + Vector3(0, 2, 0);
    m_Light.param.size = 35.0f;

    DefMatchLightManager.LightChangeAndBack(5, max(Time - 10, 1), 5, Vector3(0.5f, 0.5f, 0.5f));

    DefCamera.SetNewState(new CameraStateSkillCharacterZoom(pParent, 0.02f, m_MaxTime));
}

SpecialAttackEffect::~SpecialAttackEffect()
{

}

bool SpecialAttackEffect::Update()
{
    bool isLive = ++m_Timer < m_MaxTime;

    if (isLive)
    {
        m_pParent->m_Renderer.m_Lighting = Vector3(1, 1, 1)*0.6f;
        //m_pParent->m_Renderer.m_HDR = Vector3(1, 1, 1)*0.1f;
    }
    else
    {
        m_pParent->m_Renderer.m_Lighting = Vector3Zero; 
        m_pParent->m_Renderer.m_HDR = Vector3Zero;
    }

    return isLive;
}

bool SpecialAttackEffect::Msg(MsgType mt)
{
    return false;
}