//*****************************************************************************************************************************
//
//		オプションシーン
//
//*****************************************************************************************************************************
#pragma once
#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"

class SceneOption : public Scene
{
public:
    SceneOption();
    ~SceneOption();

    void Update();
    void Render();
};