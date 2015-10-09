#pragma once

#include "../Library/Bullet/BulletSystem.h"
#include "../GameSystem/ForwardDecl.h"

//----------------------------------------------------------//
//�@�@�@�L�����N�^�[�̕�������N���X
//----------------------------------------------------------//

class CharacterPhysic
{
public:
    CharacterPhysic(CharacterBase*  pOwnerCharacter);
    ~CharacterPhysic();

    void Update();

private:
    CharacterBase*  m_pOwnerCharacter;
    RigidBody*      m_pRigidBody;
};