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
#include "../GameScene/SceneGamePlay.h"

class SceneCharacterSelect : public Scene
{
public:
    SceneCharacterSelect(
        sceneGamePlay::InitParams& LoadParams,
        UINT PlayerNum
        );

    ~SceneCharacterSelect();

    void Update();
    void Render();

private:
    enum class NextSceneType
    {
        _NoChange,

        Back,
        Next,
    };

    std::array<SelectPointBase*, 4> m_ChrPoint;
    sceneGamePlay::InitParams m_LoadParams;
    NextSceneType   m_NextSceneType;
    const UINT      m_PlayerNum;
    CursorManager*  m_pManager;
    iex2DObj        m_Texture;
    int             m_Timer;



    void(SceneCharacterSelect::*m_pStateFunc)();
    
    void InitData();
    void SetComputerPoint(UINT Num);
    void SetCharacterPoint(std::array<SelectPointBase*, 4>& OutChrPoint);
    void SetOtherComputerCharacter();
    void CreateWindows();
    void CreateLight();

    void State_PreSelect();
    void State_Selecting();
    void State_FadeOut();
    void State_Change();

};

