#pragma once

#include "../Library/Bullet/BulletSystem.h"
#include "../GameSystem/ForwardDecl.h"

//----------------------------------------------------------//
//　　　キャラクターの物理判定クラス
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