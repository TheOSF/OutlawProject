#ifndef __PARTICLE_BASE_H__
#define __PARTICLE_BASE_H__

#include "iextreme.h"
#include "../utillity/ColorUtility.h"

//----------------------------------------------------
//  �p�[�e�B�N���̊�{�p�����[�^�N���X
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

    //�R�}�ǂ����UV�l�ɃZ�b�g����
    void SetCellUV(
        int num_cell_X,     //�e�N�X�`����X�R�}��
        int num_cell_Y,     //�e�N�X�`����Y�R�}��
        int set_cell_number //�Z�b�g����R�}�ԍ�
        );

    //���_��ݒ肷��
    void CalcParticleVertex(LPLVERTEX v);
};

#endif