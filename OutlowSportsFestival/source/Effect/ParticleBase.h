#ifndef __PARTICLE_BASE_H__
#define __PARTICLE_BASE_H__

#include "iextreme.h"
#include "../utillity/ColorUtility.h"

//----------------------------------------------------
//  パーティクルの基本パラメータクラス
//----------------------------------------------------

class ParticleBase
{
public:
    struct Param
    {
        Vector3 pos;
        Vector2 size;

        COLORf  color;
        DWORD   dw_Flag;

        float   TU[4];
        float   TV[4];
    };

    iex2DObj* m_pTexture;
    Param     m_Param;

    ParticleBase();
    virtual ~ParticleBase();

    //コマどおりのUV値にセットする
    void SetCellUV(
        int num_cell_X,     //テクスチャのXコマ数
        int num_cell_Y,     //テクスチャのYコマ数
        int set_cell_number //セットするコマ番号
        );

    //頂点を設定する
    void CalcParticleVertex(LPLVERTEX v);
};

#endif