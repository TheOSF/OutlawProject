//*****************************************************************************************************************************
//
//		キャラクタセレクトシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "CursorObject.h"

class SceneCharacterSelect : public Scene
{
public:
    SceneCharacterSelect(
        UINT PlayerNum
        );

    ~SceneCharacterSelect();

    void Update();
    void Render();

private:
    CursorManager*  m_pManager;
    CursorObject*   m_pCursor;
    iex2DObj        m_Texture;
};