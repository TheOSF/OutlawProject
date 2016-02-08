#include "SceneLoading.h"
#include "../GameSystem/GameController.h"
#include "../Sound/Sound.h"
#include "../IexSystem/Framework.h"

SceneLoading::SceneLoading(sceneGamePlay::InitParams& param) :
m_BackTexture("DATA\\LOADING\\HowToPlay.png"),
m_PushButton("DATA\\LOADING\\PleaseButton.png"),
m_Succes(false)
{
    m_Param = param;

    m_NowState = State::FadeIn;

    m_Timer = 0;

    m_PleaseButtonAlpha = 0.0f;

    m_FadeAlpha = 1.0f; //ÇÕÇ∂ÇﬂÇÕçïÇ¢

    m_pNextScene = nullptr;
}

SceneLoading::~SceneLoading()
{
    if (m_Succes == false && m_pNextScene != nullptr)
    {
        delete m_pNextScene;
    }
}

bool SceneLoading::Initialize()
{
    return true;
}

void SceneLoading::Update()
{
    switch (m_NowState)
    {
    case State::FadeIn:

        m_FadeAlpha -= 0.03f;
        if (m_FadeAlpha < 0)
        {
            m_NowState = State::Loading;
        }

        break;

    case State::Loading:

        m_pNextScene = new sceneGamePlay(m_Param);
        m_pNextScene->Initialize();

        m_NowState = State::LoadFinish;

        break;

    case State::LoadFinish:
        m_Timer++;

        m_PleaseButtonAlpha = sinf((float)m_Timer*0.1f)*0.5f + 0.5f;

        if (m_Timer > 100000)
        {
            m_Timer = 0;
        }

        if (controller::GetPushAnyController(controller::button::maru))
        {
            m_NowState = State::FadeOut;
            Sound::Play(Sound::Scene_Enter);
        }

        break;

    case State::FadeOut:

        m_PleaseButtonAlpha -= 0.02f;

        m_FadeAlpha += 0.03f;

        if (m_FadeAlpha > 1.2f)
        {
            m_Succes = true;
            MainFrame->SwapScene(m_pNextScene);
        }

        break;

    default:
        MyAssert(false, "ílÇ™ïsê≥Ç≈Ç∑");
        break;
    }
}

void SceneLoading::Render()
{
    
    //îwåiï`âÊ
    m_BackTexture.Render();

    //PleaseButtonï`âÊ
    m_PushButton.Render(
        818,
        586,
        512,
        128,

        0,
        0,
        512,
        128,
        RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, fClamp(m_PleaseButtonAlpha, 1, 0))
        );

    //ëSëÃÇçïÇ≠
    iexPolygon::Rect(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, fClamp(m_FadeAlpha, 1, 0))
        );
}