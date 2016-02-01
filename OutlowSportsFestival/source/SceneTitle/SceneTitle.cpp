
#include    "SceneTitle.h"
#include    "../Sound/Sound.h"
#include    "../GameSystem/GameController.h"
#include    "../IexSystem/Framework.h"
#include    "../SceneOption/SceneOption.h"

SceneTitle::SceneTitle()
{
    m_GoNext = false;

    m_PushButton_AlphaCount = 0.0f;
    m_FadeOut = 0.0f;

    m_Timer = 0;


    m_pTitleLogoTex  = new iex2DObj("DATA\\TITLE\\TitleLogo.png");
    m_pPushButtonTex = new iex2DObj("DATA\\TITLE\\PleaseButton.png");
    m_pBackTex = new iex2DObj("DATA\\Texture\\sports.png");

    {
        iexLight::SetFog(800, 1000, 0);
    }
}

SceneTitle::~SceneTitle()
{
    delete m_pTitleLogoTex;
    delete m_pPushButtonTex;
    delete m_pBackTex;
}

void SceneTitle::Update()
{

    m_PushButton_AlphaCount += 0.1f;

    if (m_GoNext == false && controller::GetTRG(controller::button::maru, 0))
    {
        m_GoNext = true;

        //SE
        Sound::Play(Sound::Scene_Enter);
    }

    if (m_GoNext)
    {
        ++m_Timer;

        m_PushButton_AlphaCount += 0.5f;
    }

    if (m_Timer > 40)
    {
        m_FadeOut += 0.05f;
    }

    if (m_Timer > 60)
    {
        //オプションセレクトへ
        MainFrame->ChangeScene(new SceneOption());
    }
    
}

void SceneTitle::Render()
{
    iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

    m_pBackTex->Render();

    {
        const int W = 700, H = 700;

        m_pTitleLogoTex->Render(
            iexSystem::ScreenWidth / 2 - W / 2,
            iexSystem::ScreenHeight / 2 - H / 2,
            W,
            H,
            0,
            0,
            1000,
            1000
            );
    }

    {
        const int W = 512, H = 128;
        float Alpha = sinf(m_PushButton_AlphaCount)*0.5f + 0.5f;

        Alpha = fClamp(Alpha, 1, 0);

        m_pPushButtonTex->Render(
            iexSystem::ScreenWidth / 2 - W / 2,
            600,
            W,
            H,
            0,
            0,
            512,
            128,
            RS_COPY,
            D3DCOLOR_COLORVALUE(1, 1, 1, Alpha)
            );
    }

    iexPolygon::Rect(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        RS_COPY,
        D3DCOLOR_COLORVALUE(0,0,0, fClamp(m_FadeOut, 1, 0))
        );

    
}