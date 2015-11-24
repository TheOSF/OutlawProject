//*****************************************************************************************************************************
//
//		タイトルシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"

class SceneTitle : public Scene
{
public:
    SceneTitle();
    ~SceneTitle();

    void Update();
    void Render();

};