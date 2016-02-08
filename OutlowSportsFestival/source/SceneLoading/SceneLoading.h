
#pragma once

#include "../IexSystem/Scene.h"
#include "../GameScene/SceneGamePlay.h"


class SceneLoading : public Scene
{
public:
    SceneLoading(sceneGamePlay::InitParams& param);
    ~SceneLoading();


    bool Initialize()override;

    void Update()override;
    void Render()override;


private:

    enum class State
    {
        FadeIn,
        Loading,
        LoadFinish,
        FadeOut
    };

    sceneGamePlay::InitParams m_Param;

    iex2DObj    m_BackTexture;
    iex2DObj    m_PushButton;
    State       m_NowState;
    int         m_Timer;
    RATIO       m_PleaseButtonAlpha;
    RATIO       m_FadeAlpha;
    bool        m_Succes;

    sceneGamePlay* m_pNextScene;
};