#pragma once

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"
#include "CharacterRenderer.h"

//***************************************************
//		�L�����N�^�[���� �|�[�Y���Ƃ�N���X
//***************************************************
class CharacterPoseMotion
{
public:

    // �R���X�g���N�^
    CharacterPoseMotion(
        CharacterBase*     pCharacter,
        RADIAN             FrontViewSpeed
        );

    // �f�X�g���N�^
    ~CharacterPoseMotion();

    // �X�V
    void Update();

private:
    CharacterBase*  const       m_pCharacter;  // �L�����N�^�[
    const RADIAN                m_FrontViewSpeed; //���ʂ���������
};

