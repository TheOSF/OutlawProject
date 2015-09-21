#pragma once

#include "../Render/Renderer.h"
#include "iextreme.h"
#include "../utillity/ColorUtility.h"

//---------------------------------------------
//�@�|���S���A�j���[�V�����`��N���X
//---------------------------------------------

class AnimationBordRenderer :protected ForwardRenderer
{
public:

    AnimationBordRenderer(
        LPIEX2DOBJ      pTexture, //�e�N�X�`��
        int             NumCellX, //�A�j���[�V�����R�}����
        int             NumCellY, //�A�j���[�V�����R�}���c
        int             MaxCell,  //���R�}��
        COLORf          Color,    //�F
        DWORD           dwFlags   //�`�惂�[�h
        );

    ~AnimationBordRenderer();

    Vector3   m_Pos;
    Vector3   m_Right, m_Up;
    Vector2   m_Size;
              
    int       m_CellCount;

private:

    LPIEX2DOBJ      m_pTexture;
    int             m_NumCellX;
    int             m_NumCellY;
    int             m_MaxCell;
    DWORD           m_Color;
    DWORD           m_DwFlags;

    void CalcZ() override;
    void Render()override;
};