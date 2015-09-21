#pragma once

#include "../Render/Renderer.h"
#include "iextreme.h"
#include "../utillity/ColorUtility.h"

//---------------------------------------------
//　板ポリゴンアニメーション描画クラス
//---------------------------------------------

class AnimationBordRenderer :protected ForwardRenderer
{
public:

    AnimationBordRenderer(
        LPIEX2DOBJ      pTexture, //テクスチャ
        int             NumCellX, //アニメーションコマ数横
        int             NumCellY, //アニメーションコマ数縦
        int             MaxCell,  //総コマ数
        COLORf          Color,    //色
        DWORD           dwFlags   //描画モード
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