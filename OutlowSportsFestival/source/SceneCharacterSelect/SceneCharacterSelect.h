//*****************************************************************************************************************************
//
//		キャラクタセレクトシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "CursorObject.h"
#include "../GameSystem/GameSystem.h"

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
    iex2DObj        m_Texture;

    void SetComputerPoint(UINT Num);
    void SetCharacterPoint(std::array<SelectPointBase*, 4>& OutChrPoint);

    void State_PreSelect();
    void State_Selecting();
    void State_FadeOut();
    void State_Change();

};

