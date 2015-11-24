#include "PlayerGauge.h"
#include "../GameSystem/ResourceManager.h"


PlayerGauge::PlayerGauge(CharacterBase* pOwnerCharacter) :
m_pOwnerCharacter(pOwnerCharacter)
{
    const int ScreenLeftSpace = 50;

    m_Renderer.m_Size = Vector2(1.00f, 1.00f);
    m_Renderer.m_Color     = COLORf(1, 1, 1, 1);
    m_Renderer.m_FaceColor = COLORf(1, 1, 1, 1);

    m_Renderer.m_Pos.x = ScreenLeftSpace+(float)((int)pOwnerCharacter->m_PlayerInfo.number * ((iexSystem::ScreenWidth - ScreenLeftSpace * 2) / 4));
    m_Renderer.m_Pos.y = 765;

    m_Renderer.m_Num = pOwnerCharacter->m_PlayerInfo.number;
    m_Renderer.m_PlayerType = pOwnerCharacter->m_PlayerInfo.player_type;

    m_RoundCount = m_pOwnerCharacter->m_Params.win;
}

PlayerGauge::~PlayerGauge()
{

}


bool PlayerGauge::Update()
{
    //ゲージの伸び縮み
    const float Speed = 0.3f;

    m_Renderer.m_Life += ((m_pOwnerCharacter->m_Params.HP / m_pOwnerCharacter->m_Params.maxHP) - m_Renderer.m_Life) * Speed;
    m_Renderer.m_Sp += (m_pOwnerCharacter->m_Params.SP - m_Renderer.m_Sp) * Speed;


    //ラウンドカウントが変化していた場合アイコンを足す
    if (m_RoundCount < m_pOwnerCharacter->m_Params.win)
    {
        m_RoundCount = m_pOwnerCharacter->m_Params.win;
        m_Renderer.AddRoundIcon(Vector2((float)iexSystem::ScreenWidth / 2, (float)iexSystem::ScreenHeight / 2 - 100));
    }

    //アイコン移動更新
    m_Renderer.UpdateRoundIcon();

    return true;
}


bool PlayerGauge::Msg(MsgType mt)
{

    return false;
}

//-----------------------------------------------------------
//  ゲージレンダラー
//-----------------------------------------------------------

PlayerGauge::GaugeRenderer::GaugeRenderer() :
m_Life(0),
m_Sp(0),
m_Pos(0, 0),
m_Size(1, 1)
{
    m_pUItex = DefResource.Get(Resource::TextureType::UI_gauges);
    m_pIconTex = DefResource.Get(Resource::TextureType::UI_icon);

    m_RoundIconParams.fill({ false, 0, 1, Vector2(0, 0) });

}

void PlayerGauge::GaugeRenderer::AddRoundIcon(CrVector2 Pos)
{
    for (int i = 0; i < RoundCountMax; ++i)
    {
        //空きラウンドカウントにセットする
        if (m_RoundIconParams[i].Enable == false)
        {
            m_RoundIconParams[i].Enable = true;
            m_RoundIconParams[i].Frame = 0;
            m_RoundIconParams[i].Pos = Pos;
            m_RoundIconParams[i].Size = 2.0f;
            return;
        }
    }

    MyAssert(false, "UI1のラウンドアイコン表示が最大数に達しています");
    
}

void PlayerGauge::GaugeRenderer::BackRender()
{

    //全体の背景
    m_pUItex->Render(
        (int)m_Pos.x,
        (int)m_Pos.y,
        (int)(400.0f*m_Size.x),
        (int)(100.0f*m_Size.y),

        0,
        0,
        400,
        99
        );
}

void PlayerGauge::GaugeRenderer::LifeRender()
{
    const float SizeX = 191;
    const float SizeY = 42;
    const float StartX = 80.0f;
    const float StartY = 1.0f;

    //ライフバーの背景
    m_pUItex->Render(
        (int)m_Pos.x + (int)(StartX*m_Size.x),
        (int)m_Pos.y + (int)(StartY*m_Size.y),
        (int)(SizeX*m_Size.x),
        (int)(SizeY*m_Size.y),

        0,
        100,
        (int)SizeX,
        (int)SizeY
        );

    //ライフバー
    m_pUItex->Render(
        (int)m_Pos.x + (int)(StartX*m_Size.x),
        (int)m_Pos.y + (int)(StartY*m_Size.y),
        (int)(SizeX*m_Size.x* m_Life),
        (int)(SizeY*m_Size.y),

        200,
        100,
        (int)(SizeX * m_Life), 
        (int)SizeY
        );

}

void PlayerGauge::GaugeRenderer::SpRender()
{
    const float SizeX = 200;
    const float SizeY = 10;
    const float StartX = 106.0f;
    const float StartY = 45.0f;

    //SPバーの背景
    m_pUItex->Render(
        (int)m_Pos.x + (int)(StartX*m_Size.x),
        (int)m_Pos.y + (int)(StartY*m_Size.y),
        (int)(SizeX*m_Size.x),
        (int)(SizeY*m_Size.y),

        0,
        150,
        (int)SizeX,
        (int)SizeY
        );

    //SPバー
    m_pUItex->Render(
        (int)m_Pos.x + (int)(StartX*m_Size.x),
        (int)m_Pos.y + (int)(StartY*m_Size.y),
        (int)(SizeX*m_Size.x* m_Sp),
        (int)(SizeY*m_Size.y),

        0,
        180,
        (int)(SizeX * m_Sp),
        (int)SizeY
        );
}

void PlayerGauge::GaugeRenderer::FaceRender()
{
    return;///////////////

    const float SizeX = 200;
    const float SizeY = 10;
    const float StartX = 106.0f;
    const float StartY = 45.0f;

    m_pIconTex->Render(
        (int)m_Pos.x + (int)(StartX*m_Size.x),
        (int)m_Pos.y + (int)(StartY*m_Size.y),
        (int)(SizeX*m_Size.x* m_Sp),
        (int)(SizeY*m_Size.y),

        0,
        0,
        (int)(SizeX * m_Sp),
        (int)SizeY
        );
}

void PlayerGauge::GaugeRenderer::PlayerNumRender()
{
    const float SizeX = 55;
    const float SizeY = 42;

    int TexX = 0;
    int TexY = 0;

    if (m_PlayerType == PlayerType::_Player)
    {
        TexX = ((int)m_Num % 3) * 70;
        TexY = 260 + ((int)m_Num / 3);
    }
    else
    {
        TexX = 70;
        TexY = 310;
    }

    m_pUItex->Render(
        (int)m_Pos.x + (int)(272*m_Size.x),
        (int)m_Pos.y + (int)(1*m_Size.y),
        (int)(SizeX*m_Size.x),
        (int)(SizeY*m_Size.y),

        TexX,
        TexY,
        (int)70,
        (int)50
        );
}


void PlayerGauge::GaugeRenderer::WinIconRender(CrVector2 pos, float Size)
{
    const float SizeX = 50 * m_Size.x*Size;
    const float SizeY = 50 * m_Size.y*Size;


    m_pUItex->Render(
        (int)(pos.x - SizeX*0.5f),
        (int)(pos.y - SizeY*0.5f),
        (int)SizeX,
        (int)SizeY,

        0,
        210,
        50,
        50
        );
}

void PlayerGauge::GaugeRenderer::AllWinIconRender()
{
    for (int i = 0; i < RoundCountMax; ++i)
    {
        auto& it = m_RoundIconParams.at(i);
        if (it.Enable == false)
        {
            break;
        }

        WinIconRender(it.Pos, it.Size);
    }
}

//アイコン位置のゲッタ
Vector2 PlayerGauge::GaugeRenderer::CalcIconPos(int n)
{
    Vector2 Pos(135 * m_Size.x, 85 * m_Size.y);
    const float Space = 55 * m_Size.x;

    Pos += m_Pos;
    Pos.x += Space*(float)n;

    return Pos;
}

void PlayerGauge::GaugeRenderer::UpdateRoundIcon()
{
    Vector2 v;
    const float Speed = 0.05f;
    const int MoveFrame = 60;

    for (int i = 0; i < RoundCountMax; ++i)
    {
        auto& it = m_RoundIconParams.at(i);

        if (it.Enable == false)
        {
            break;
        }

        if (it.Frame > MoveFrame)
        {
            it.Pos += (CalcIconPos(i) - it.Pos)*Speed;
            it.Size += (1 - it.Size)*Speed;
        }
        else
        {
            ++it.Frame;
        }
    }
}

void PlayerGauge::GaugeRenderer::CalcZ()
{
    m_SortZ = -1;
}

void PlayerGauge::GaugeRenderer::Render()
{
    LifeRender();
    SpRender();
    BackRender();
    PlayerNumRender();
    FaceRender();
    AllWinIconRender();
}