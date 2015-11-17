#include "PlayerGauge.h"
#include "../GameSystem/ResourceManager.h"


PlayerGauge::PlayerGauge(CharacterBase* pOwnerCharacter) :
m_pOwnerCharacter(pOwnerCharacter)
{
    const int ScreenLeftSpace = 50;

    m_Renderer.m_Size = Vector2(1.05f, 1.05f);
    m_Renderer.m_Color     = COLORf(1, 1, 1, 1);
    m_Renderer.m_FaceColor = COLORf(1, 1, 1, 1);

    m_Renderer.m_Pos.x = ScreenLeftSpace+(float)((int)pOwnerCharacter->m_PlayerInfo.number * ((iexSystem::ScreenWidth - ScreenLeftSpace * 2) / 4));
    m_Renderer.m_Pos.y = 765;

    m_Renderer.m_Num = pOwnerCharacter->m_PlayerInfo.number;
    m_Renderer.m_PlayerType = pOwnerCharacter->m_PlayerInfo.player_type;
}

PlayerGauge::~PlayerGauge()
{

}


bool PlayerGauge::Update()
{
    const float Speed = 0.3f;

    m_Renderer.m_Life += ((m_pOwnerCharacter->m_Params.HP / m_pOwnerCharacter->m_Params.maxHP) - m_Renderer.m_Life) * Speed;
    m_Renderer.m_Sp += (m_pOwnerCharacter->m_Params.SP - m_Renderer.m_Sp) * Speed;

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
}