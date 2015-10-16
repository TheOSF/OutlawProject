#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


//-------------------------------------------------------
//  �Q�[���Z�b�g�\���N���X
//-------------------------------------------------------

class GameSetUI :public UserInterfaceRenderer, public GameObjectBase
{
public:
    GameSetUI();
    ~GameSetUI();

private:
    const float  m_Z;
    iex2DObj*    m_pTexture;

    float        m_Count;
    float        m_T;

    void CalcZ();
    //�`��(�����I�ɌĂ΂��)
    void Render();

    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt)
    {
        return false;
    }

};