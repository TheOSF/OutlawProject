#include "CharacterPoseMotion.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

// �R���X�g���N�^
CharacterPoseMotion::CharacterPoseMotion(
    CharacterBase*     pCharacter,
    RADIAN             FrontViewSpeed
    ) :
    m_pCharacter(pCharacter),
    m_FrontViewSpeed(FrontViewSpeed)
{

}


CharacterPoseMotion::~CharacterPoseMotion()
{

}

// �X�V
void CharacterPoseMotion::Update()
{
    chr_func::AngleControll(m_pCharacter, m_pCharacter->m_Params.pos - Vector3AxisZ, m_FrontViewSpeed);
    chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
}
