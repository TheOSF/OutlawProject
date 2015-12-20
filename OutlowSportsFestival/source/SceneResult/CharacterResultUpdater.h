#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterRenderer.h"

//-------------------------------------------------------
//  リザルト中のキャラクタ更新クラス
//-------------------------------------------------------

class CharacterResultUpdater : public GameObjectBase
{
public:
    CharacterResultUpdater(
        CharacterRenderer* pRenderer
        );

    ~CharacterResultUpdater();

private:

    CharacterRenderer* const m_pRenderer;

    bool Update();
    bool Msg(MsgType mt)
    {
        return false;
    }
};