#pragma once

//------------------------------------------------------------//
// �ėp�t�h�`��N���X
//------------------------------------------------------------//

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"
#include "StateTemplate.h"

class UIbase :public UserInterfaceRenderer, public GameObjectBase
{
public:
    typedef State<UIbase, void> State;

    //�`��^�C�v��
    enum class DrawType
    {
        Usual,  //�ʏ�
        Add,    //���Z
        Sub     //���Z
    };

    UIbase();
    ~UIbase();

    void SetState(State* NewState);  //�V�����X�e�[�g���Z�b�g

    struct
    {
        bool      Delete;     //�폜�t���O(true��Delete�����)
        iex2DObj* Texture;    //�e�N�X�`���ւ̃|�C���^
        RectI     UV;         //�e�N�X�`���̃T���v�����O�����`���W
        Vector3   pos;        //���W(x = ��ʍ��W�@y = ��ʍ��W�@z = z���O ���s��(�`�揇��) )
        Vector2   scale;      //�X�P�[��
        RADIAN    angle;      //�A���O��
        DWORD     color;      //�F
        DrawType  dw_type;    //�`��^�C�v

    }param;

private:
    typedef StateMachine<UIbase, void> MyStateMacihne;

    MyStateMacihne* m_pStateMachine;

    void CalcZ();
    void Render();
    bool Update();
    bool Msg(MsgType mt);
};