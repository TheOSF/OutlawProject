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
    class Renderer
    {
    public:
        enum class DrawType
        {
            
        };

        void Render(

            );
    };
    SceneOption();
    ~SceneOption();

    void Update();
    void Render();
};