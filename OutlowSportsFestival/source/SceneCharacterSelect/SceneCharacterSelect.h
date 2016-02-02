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

class SceneCharacterSelectUI;

class SceneCharacterSelect : public Scene {
public:
    SceneCharacterSelect(
        sceneGamePlay::InitParams& LoadParams,
        UINT PlayerNum
        );

    ~SceneCharacterSelect();

    bool Initialize();

    void Update();
    void Render();

private:
    enum class NextSceneType {
        _NoChange,

        Back,
        Next,
    };

    std::array<SelectPointBase*, 4> m_ChrPoint;
    std::array<SelectCursor*, 4> m_Cursor;
    sceneGamePlay::InitParams m_LoadParams;
    NextSceneType   m_NextSceneType;
    const UINT      m_PlayerNum;
    CursorManager*  m_pManager;
    iex2DObj        m_Texture;
    int             m_Timer;
    iex2DObj        m_BackTex;
    iex2DObj m_OK_Tex;
    SelectPointBase*m_pComputerDefaultPoint;
    SceneCharacterSelectUI* m_pOK_UI;

    void(SceneCharacterSelect::*m_pStateFunc)();

    void InitData();
    void SetComputerPoint();
    void SetComputerStrongPoint();
    void SetSceneUI(); // シーンを構成するパーツを作成
    void SetCharacterPoint();
    void SetOtherComputerCharacter();
    void SetCursor();
    void CreateLight();
    void CreateFloor();
    void CreateBack();

    void State_PreSelect();
    void State_Selecting();
    void State_Confirm();
    void State_FadeOut();
    void State_BackToOption();
    void State_Change();


};
