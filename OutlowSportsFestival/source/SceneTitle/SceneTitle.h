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

private:

    iex2DObj*   m_pTitleLogoTex;
    iex2DObj*   m_pPushButtonTex;
    iex2DObj*   m_pBackTex;

    float       m_PushButton_AlphaCount;
    RATIO       m_FadeOut;
    int         m_Timer;
    bool        m_GoNext;
};