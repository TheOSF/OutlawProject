//*****************************************************************************************************************************
//
//		ステージセレクトシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "../GameSystem/GameSystem.h"

#include "../GameScene/SceneGamePlay.h"

class SceneStageSelect : public Scene
{
public:
    SceneStageSelect(
        sceneGamePlay::InitParams& LoadParams
        );

    ~SceneStageSelect();

    void Update();
    void Render();

private:
    enum class NextSceneType
    {
        _NoChange,

        Back,
        Next,
    };

    NextSceneType               m_NextSceneType;
    sceneGamePlay::InitParams   m_LoadParams;
    int     m_Timer;

    void(SceneStageSelect::*m_pStateFunc)();

    void State_PreSelect();
    void State_Selecting();
    void State_FadeOut();
    void State_Change();

};

