#include "SceneCharacterSelectUI.h"


SceneCharacterSelectUI::Param::Param() :
    Pos(0, 0),
    Size(0, 0),
    Color(1, 1, 1, 1)
{

}

SceneCharacterSelectUI::SceneCharacterSelectUI(iex2DObj* const pTexture, const RectI& TextureRect) :
    m_pTexture(pTexture),
    m_TextureRect(TextureRect),
    m_Visible(true),
    m_Z(0)
{

}


SceneCharacterSelectUI::~SceneCharacterSelectUI()
{

}

// ���W�X�V
void SceneCharacterSelectUI::PositionUpdate()
{
    const float Speed = 0.2f;

    m_Current.Pos += (m_Target.Pos - m_Current.Pos)*Speed;
}

// �T�C�Y�X�V
void SceneCharacterSelectUI::SizeUpdate()
{
    const float Speed = 0.2f;

    m_Current.Size += (m_Target.Size - m_Current.Size)*Speed;
}

// �J���[�X�V
void SceneCharacterSelectUI::ColorUpdate()
{
    const float Speed = 0.2f;

    m_Current.Color += (m_Target.Color - m_Current.Color)*Speed;
}

// �X�V
bool SceneCharacterSelectUI::Update()
{
    // ���W�X�V
    PositionUpdate();

    // �T�C�Y�X�V
    SizeUpdate();

    // �J���[�X�V
    ColorUpdate();

    return true;
}

// �`��
void SceneCharacterSelectUI::Render()
{
    if ( m_pTexture == nullptr || m_Visible == false )return;

    const int PosX = (int)(m_Current.Pos.x - m_Current.Size.x / 2.0f);
    const int PosY = (int)(m_Current.Pos.y - m_Current.Size.y / 2.0f);

    const int SizeX = (int)(m_Current.Size.x);
    const int SizeY = (int)(m_Current.Size.y);

    m_pTexture->Render(
        PosX,
        PosY,
        SizeX,
        SizeY,
        m_TextureRect.x,
        m_TextureRect.y,
        m_TextureRect.w,
        m_TextureRect.h,
        RS_COPY,
        D3DCOLOR_COLORVALUE(m_Current.Color.x, m_Current.Color.y, m_Current.Color.z, m_Current.Color.w)
        );
}


void SceneCharacterSelectUI::CalcZ()
{
    m_SortZ = m_Z;
}


