#pragma once

#include "iextreme.h"

//------------------------------------------//
// �@�@�@�p�[�e�B�N���Ǘ��N���X
//------------------------------------------//

class ParticleManager
{
public:

    //�p�[�e�B�N���f�[�^�\����
    struct ParticleData
    {
        UINT        count;

        UINT        type;
        Vector3     pos;
        Vector3     move;
        Vector3     power;
        Vector2     size;

        UINT        md_frame;
        UINT        end_frame;

        DWORD       start_color;
        DWORD       md_color;
        DWORD       end_color;

        DWORD       dw_flag;

        float       cameraZ;
    };

    //������
    static void Initialze(
        UINT         maxParticle,  //�p�[�e�B�N���ő吔
        iex2DObj*    pTexture,     //�p�[�e�B�N���e�N�X�`��
        UINT         TextureCellX, //�e�N�X�`���R�}���w
        UINT         TextureCellY  //�e�N�X�`���R�}���w
        );

    //�C���X�^���X�擾
    static ParticleManager& GetInstance();

    //�C���X�^���X�J��
    static void Release();


    //�p�[�e�B�N���Z�b�g
    void Set(
        UINT        type,        //�p�[�e�B�N���R�}�ԍ�
        CrVector3   pos,         //�o���ꏊ
        CrVector3   move,        //�����ړ���
        CrVector3   power,       //�͏�(�d�͂Ƃ�)
        CrVector2   size,        //�傫���w�x

        UINT        md_frame,    //���ԃt���[��
        UINT        end_frame,   //�����t���[��

        DWORD       start_color, //�����J���[
        DWORD       md_color,    //���ԃt���[���ł̃J���[
        DWORD       end_color,   //�I�����̃J���[
        DWORD       dw_flag      //�`��t���O
        );

    //�p�[�e�B�N���Z�b�g
    void Set(
        UINT        type,     //�p�[�e�B�N���R�}�ԍ�
        CrVector3   pos,      //�o���ꏊ
        CrVector3   move,     //�����ړ���
        CrVector3   power,    //�͏�(�d�͂Ƃ�)
        CrVector2   size,     //�傫���w�x
        UINT        end_frame,//�����t���[��
        DWORD       color = 0xFFFFFFFF, //�F
        DWORD       dw_flag = RS_COPY //�`��t���O
        );





    //�`��
    void Render();

    //�X�V
    void Update();





private:

    

    ParticleManager(
        UINT         maxParticle,  //�p�[�e�B�N���ő吔
        iex2DObj*    pTexture,     //�p�[�e�B�N���e�N�X�`��
        UINT         TextureCellX, //�e�N�X�`���R�}���w
        UINT         TextureCellY  //�e�N�X�`���R�}���w
        );

    ~ParticleManager();

    //�\�[�g
    void ParticleSort();


    static ParticleManager* m_pInstance;

    const UINT         m_MaxParticle;  //�p�[�e�B�N���ő吔
    iex2DObj* const    m_pTexture;     //�p�[�e�B�N���e�N�X�`��
    const UINT         m_TextureCellX; //�e�N�X�`���R�}���w
    const UINT         m_TextureCellY;  //�e�N�X�`���R�}���w

    ParticleData*      m_ParticleDataArray;
    ParticleData**     m_pWorkParticleDataPointerArray;
    int                m_SetCount;
};


#define DefParticle (ParticleManager::GetInstance())