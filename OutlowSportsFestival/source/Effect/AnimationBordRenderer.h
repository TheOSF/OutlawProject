#pragma once

#include "../Render/Renderer.h"
#include "iextreme.h"
#include "../utillity/ColorUtility.h"
#include "../GameSystem/GameObject.h"

//---------------------------------------------
//�@�|���S���A�j���[�V�����`��N���X
//---------------------------------------------

class AnimationBordRenderer :protected ForwardHDRRenderer
{
public:

    AnimationBordRenderer(
        LPIEX2DOBJ      pTexture, //�e�N�X�`��
        int             NumCellX, //�A�j���[�V�����R�}����
        int             NumCellY, //�A�j���[�V�����R�}���c
        int             MaxCell,  //���R�}��
        COLORf          Color,    //�F
        DWORD           hdr       //HDRcolor
        );

    ~AnimationBordRenderer();

    int GetMaxCell()const;

    Vector3   m_Pos;          //�̒��S���W
    Vector3   m_Right, m_Up;  //�̉E�����ƁA��������
    Vector2   m_Size;         //�̑傫��

    int       m_CellCount;    //�ɂ͂�e�N�X�`���̃R�}�ԍ�

private:

    LPIEX2DOBJ      m_pTexture;
    int             m_NumCellX;
    int             m_NumCellY;
    int             m_MaxCell;
    DWORD           m_Color;
    DWORD           m_HdrCol;

    void CalcZ() override;
    void Render()override;
};



//---------------------------------------------------------
//�@�|���S���A�j���[�V�����`��N���X�𓮂����N���X
//---------------------------------------------------------

class AnimationBordGameObj :public GameObjectBase
{
public:
    static const int m_Infinite_Livetime = INT_MAX;   //�����t���[���𖳌��Ɏw�肷��ꍇ�̒萔


    Vector3 move_speed;              //���W�̈ړ��X�s�[�h(���[���h���)
    Vector3 move_power;              //�p���I�ɂ������(���[���h���)
                                     
    Vector3 rotate_speed;            //��]���x(���[�J�����)
    Vector3 rotate_power;            //�p���I�ɂ������]���x(���[�J�����)
                                     
    Vector2 scale_speed;             //�傫���Ȃ�X�s�[�h(���[�J�����)
    Vector2 scale_power;             //�p���I�ɂ�����傫���Ȃ�X�s�[�h(���[�J�����)
                                     
    float   animation_speed;         //�A�j���[�V�����̃X�s�[�h(�P�łP�t���[��������P�R�}�i��)

    bool    animation_end_delete;    //�A�j���[�V�������I�����Ă���ꍇ�ɏ������邩�ǂ���
    bool    animation_loop;          //�A�j���[�V���������[�v�����邩�ǂ���


    AnimationBordGameObj(
        AnimationBordRenderer*   pRenderer,        //�`��N���X�ւ̃|�C���^(�I������delete����)
        int     live_frame = m_Infinite_Livetime   //��������(�f�t�H���g�ł͖���)
        );

    ~AnimationBordGameObj();

private:

    AnimationBordRenderer* const m_pRenderer;
    int                          m_LiveFrame;
    float                        m_MyCellCount;

    bool Update();
    bool Msg(MsgType mt);

    void UpdateFrame();
    void UpdateRotation();
    void UpdateScale();
    void UpdatePosition();
};