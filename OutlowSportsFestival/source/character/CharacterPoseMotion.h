#pragma once

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"
#include "CharacterRenderer.h"

//***************************************************
//		キャラクター共通 ポーズをとるクラス
//***************************************************
class CharacterPoseMotion
{
public:

    // コンストラクタ
    CharacterPoseMotion(
        CharacterBase*     pCharacter,
        RADIAN             FrontViewSpeed
        );

    // デストラクタ
    ~CharacterPoseMotion();

    // 更新
    void Update();

private:
    CharacterBase*  const       m_pCharacter;  // キャラクター
    const RADIAN                m_FrontViewSpeed; //正面を向く早さ
};

