#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"
#include <vector>

//--------------------------------------------------------//
// �@�ȈՃ��j���[�쐬�t�h�N���X
//--------------------------------------------------------//

class SelectMenuUI:public UserInterfaceRenderer, public GameObjectBase
{
public:

    //�J�[�\���ړ������̗�
    enum class MoveVec
    {
        Up,
        Down,

        _NoMove
    };

    //����N���X�C���^�[�t�F�[�X
    class IController
    {
    public:
        virtual ~IController(){}

        virtual SelectMenuUI::MoveVec GetVec() = 0;  //�ړ�������Ԃ��Ȃ���΂Ȃ�Ȃ�
        virtual bool isDecide() = 0;                 //�߂�l��true�Ȃ猈��
    };

    //���肵���ۂɌĂяo�����N���X�̃C���^�[�t�F�[�X
    class IDecideEvent
    {
    public:
        virtual ~IDecideEvent(){}
        virtual void Decide() = 0;
    };

    SelectMenuUI(
        IController*    pController,     //�R���g���[��(���j���[����)�N���X�ւ̃|�C���^
        iex2DObj*       pBaseTexture,    //���j���[�\�����ڂ��Q�Ƃ���e�N�X�`��
        bool            isTextureDelete, //�e�N�X�`����Delete���邩�ǂ���
        float           Z                //���s��(�`�揇��
        );

    ~SelectMenuUI();

    //���ڂ�ǉ�
    void AddMenu(
        IDecideEvent*   pDecideEvent,   //���肵���ۂɌĂяo�����
        RectI           TextureRect,    //�\������e�N�X�`���̋�`
        UINT            SizeX,          //�\������傫���w
        UINT            SizeY           //�\������傫���x
        );

private:


    //�P���ڂ̏��
    class MenuParam
    {
    public:
        MenuParam();
        ~MenuParam();

        IDecideEvent*   pDecideEvent;   //���肵���ۂɌĂяo�����
        RectI           TextureRect;    //�\������e�N�X�`���̋�`
        RATIO           SelectRatio;    //�I�����Ă���̃^�C�}�[(�\�������p)

        UINT            SizeX;          //�\������傫���w
        UINT            SizeY;          //�\������傫���x


        void SetSize(UINT X, UINT Y);
        void MulRect(RectI* pOut)const;  //�g�又�����s�������̂�Ԃ�

    };


    typedef std::vector<MenuParam> MenuParamList;

    MenuParamList       m_MenuParamList;    //���j���[���X�g
    int                 m_NowSelectNum;     //���ݑI�����Ă��鍀�ڂ̔ԍ�

    IController* const  m_pController;      //�R���g���[��(���j���[����)�N���X�ւ̃|�C���^
    iex2DObj* const     m_pBaseTexture;     //���j���[�\�����ڂ��Q�Ƃ���e�N�X�`��
    const bool          m_isTextureDelete;  //�e�N�X�`����Delete���邩�ǂ���
    const float         m_Z;                //���s��(�`�揇��

    RATIO               m_Alpha;            //UI�̓����x
    RATIO               m_BackBlack;        //UI�w�i�̍����x����

    void(SelectMenuUI::*m_pStateFunc)();    //���݂̎��s����֐��ւ̃|�C���^


    //------------------------------------//
    void State_FadeIn();
    void State_FadeOut();
    void State_CanControll();
    void State_Delete();
    //------------------------------------//


    void CalcZ();
    //�`��(�����I�ɌĂ΂��)
    void Render();

    void CalcRenderStartPos(int* OutX, int* OutY)const;   //�`��J�n�ʒu���Z�o����
    int  CalcCursorNum(MoveVec vec)const;                 //�ړ���̃J�[�\���ԍ����Z�o����
    void SelectUpdate(bool NoSelect);                     //���j���[���ڂ̍X�V

    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt)
    {
        return false;
    }

};