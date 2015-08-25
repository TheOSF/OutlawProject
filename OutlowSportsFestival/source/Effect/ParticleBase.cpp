#include "ParticleBase.h"
#include "../Camera/Camera.h"



ParticleBase::ParticleBase() :
m_pTexture(nullptr)
{
    m_Param.pos = Vector3Zero;
    m_Param.size.x = m_Param.size.y = 1;

    m_Param.color = 0xFFFFFFFF;
    m_Param.dw_Flag = RS_COPY;

    for (int i = 0; i < 4; ++i)
    {
        m_Param.TU[i] = (float)(i % 2);
        m_Param.TV[i] = (float)(i / 2);
    }
}

ParticleBase::~ParticleBase()
{

}


//コマどおりのUV値にセットする
void ParticleBase::SetCellUV(
    int num_cell_X,     //テクスチャのXコマ数
    int num_cell_Y,     //テクスチャのYコマ数
    int set_cell_number //セットするコマ番号
    )
{
    const Vector2 CellSize(1 / (float)num_cell_X, 1 / (float)num_cell_Y);
    const UINT MaxCell = num_cell_X*num_cell_Y - 1;

    set_cell_number = min(set_cell_number, MaxCell);

    m_Param.TU[0] = (float)(set_cell_number%num_cell_X)*CellSize.x;
    m_Param.TV[0] = (float)(set_cell_number / num_cell_X)*CellSize.y;

    m_Param.TU[1] = m_Param.TU[0] + CellSize.x;
    m_Param.TV[1] = m_Param.TV[0];

    m_Param.TU[2] = m_Param.TU[0];
    m_Param.TV[2] = m_Param.TV[0] + CellSize.y;

    m_Param.TU[3] = m_Param.TU[1];
    m_Param.TV[3] = m_Param.TV[2];

}

void ParticleBase::CalcParticleVertex(LPLVERTEX v)
{
    const Vector3 R = DefCamera.GetRight() * m_Param.size.x;
    const Vector3 U = DefCamera.GetUp()    * m_Param.size.y;

    const DWORD color = m_Param.color.toDWORD();

    for (int i = 0; i < 4; ++i)
    {
        v[i].color = color;

        v[i].tu = m_Param.TU[i];
        v[i].tv = m_Param.TV[i];

        v[i].x = m_Param.pos.x;
        v[i].y = m_Param.pos.y;
        v[i].z = m_Param.pos.z;
    }

    *(LPVECTOR3)(&v[0].x) += U - R;
    *(LPVECTOR3)(&v[1].x) += U + R;
    *(LPVECTOR3)(&v[2].x) += -U - R;
    *(LPVECTOR3)(&v[3].x) += R - U;

}