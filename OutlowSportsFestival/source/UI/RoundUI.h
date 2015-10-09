#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


//-------------------------------------------------------
//  ���E���h�\���N���X
//-------------------------------------------------------

class RoundUI :public ForwardRenderer, public GameObjectBase
{
public:
    RoundUI(UINT Round, float Z);
    ~RoundUI();

private:
    const float  m_Z;
    const UINT   m_Round;
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
    
    void RoundRender();
    void NumberRender(int num, int posX, int sizeX, float t);
};