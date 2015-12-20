//*****************************************************************************************************************************
//
//		キャラクタセレクトシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"

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

};