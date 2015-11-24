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
    //�Q�[�W�̐L�яk��
    const float Speed = 0.3f;

    m_Renderer.m_Life += ((m_pOwnerCharacter->m_Params.HP / m_pOwnerCharacter->m_Params.maxHP) - m_Renderer.m_Life) * Speed;
    m_Renderer.m_Sp += (m_pOwnerCharacter->m_Params.SP - m_Renderer.m_Sp) * Speed;


    //���E���h�J�E���g���ω����Ă����ꍇ�A�C�R���𑫂�
    if (m_RoundCount < m_pOwnerCharacter->m_Params.win)
    {
        m_RoundCount = m_pOwnerCharacter->m_Params.win;
        m_Renderer.AddRoundIcon(Vector2((float)iexSystem::ScreenWidth / 2, (float)iexSystem::ScreenHeight / 2 - 100));
    }

    //�A�C�R���ړ��X�V
    m_Renderer.UpdateRoundIcon();

    return true;
}


bool PlayerGauge::Msg(MsgType mt)
{

    return false;
}

//-----------------------------------------------------------
//  �Q�[�W�����_���[
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
        //�󂫃��E���h�J�E���g�ɃZ�b�g����
        if (m_RoundIconParams[i].Enable == false)
        {
            m_RoundIconParams[i].Enable = true;
            m_RoundIconParams[i].Frame = 0;
            m_RoundIconParams[i].Pos = Pos;
            m_RoundIconParams[i].Size = 2.0f;
            return;
        }
    }

    MyAssert(false, "UI1�̃��E���h�A�C�R���\�����ő吔�ɒB���Ă��܂�");
    
}

void PlayerGauge::GaugeRenderer::BackRender()
{

    //�S�̂̔w�i
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

    //���C�t�o�[�̔w�i
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

    //���C�t�o�[
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

    //SP�o�[�̔w�i
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

    //SP�o�[
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

//�A�C�R���ʒu�̃Q�b�^
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