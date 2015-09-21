#include "AnimationBordRenderer.h"
#include "../Camera/Camera.h"


AnimationBordRenderer::AnimationBordRenderer(
    LPIEX2DOBJ      pTexture,
    int             NumCellX,
    int             NumCellY,
    int             MaxCell,
    COLORf          Color,
    DWORD           dwFlags
    ) :
    m_pTexture(pTexture),
    m_NumCellX(NumCellX),
    m_NumCellY(NumCellY),
    m_MaxCell(MaxCell),
    m_Color(Color.toDWORD()),
    m_DwFlags(dwFlags)
{
    m_Pos = Vector3Zero;

    m_Right = Vector3AxisX;
    m_Up = Vector3AxisY;

    m_Size = Vector2(1, 1);

    m_CellCount = 0;
}


AnimationBordRenderer::~AnimationBordRenderer()
{

}

void AnimationBordRenderer::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Pos);
}

void AnimationBordRenderer::Render()
{
    static LVERTEX v[4];

    v[0].pos = m_Right*-m_Size.x + m_Up*m_Size.y;
    v[1].pos = m_Right* m_Size.x + m_Up*m_Size.y;
    v[2].pos = m_Right*-m_Size.x - m_Up*m_Size.y;
    v[3].pos = m_Right* m_Size.x - m_Up*m_Size.y;

    for (int i = 0; i < 4; ++i)
    {
        v[i].color = m_Color;
        v[i].pos.x += m_Pos.x;
        v[i].pos.y += m_Pos.y;
        v[i].pos.z += m_Pos.z;
    }

    const Vector2 CellSize(1 / (float)m_NumCellX, 1 / (float)m_NumCellY);

    v[0].tu = (float)(m_CellCount%m_NumCellX)*CellSize.x;
    v[0].tv = (float)(m_CellCount / m_NumCellX)*CellSize.y;

    v[1].tu = v[0].tu + CellSize.x;
    v[1].tv = v[0].tv;

    v[2].tu = v[0].tu;
    v[2].tv = v[0].tv + CellSize.y;

    v[3].tu = v[1].tu;
    v[3].tv = v[2].tv;

    iexPolygon::Render3D(v, 2, m_pTexture, m_DwFlags);
}