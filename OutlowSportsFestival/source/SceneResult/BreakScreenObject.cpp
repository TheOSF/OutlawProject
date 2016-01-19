#include "BreakScreenObject.h"
#include "../Camera/Camera.h"
#include "../GameSystem/GameController.h"


BreakScreenObject::BreakScreenObject(iex2DObj* pTexture) :
m_BreakScreenModel("DATA\\RESULT\\BreakScreen\\wall_break.iem"),
m_pTexture(pTexture),
m_Timer(0),
m_Scale(1.0f),
m_Alpha(1),
m_MoveY(0),
m_DiffPosY(0)
{

    //�ʒu��������
    CalcBreakScreenWorldMatrix(&m_BreakScreenModel.TransMatrix);

    shader->SetValue("ColorTexMap", m_pTexture->GetTexture());
}


BreakScreenObject::~BreakScreenObject()
{
    delete m_pTexture;
}

void BreakScreenObject::CalcBreakScreenWorldMatrix(Matrix* T)
{
    const float Z = 5.0f;
    const Vector3 Pos = DefCamera.m_Position + DefCamera.GetForward()*Z + Vector3(0, m_DiffPosY, 0);

    //Vector3 ScreenRight;

    ////��ʉE�[�̃��[���h���W�����߂�
    //{
    //    D3DXMATRIX M, W;

    //    D3DXMatrixIdentity(&M);

    //    {
    //        D3DXMatrixIdentity(&W);

    //        W._11 = (float)iexSystem::ScreenWidth*0.5f;
    //        W._22 = -(float)iexSystem::ScreenHeight*0.5f;

    //        W._41 = W._11;
    //        W._42 = -W._22;

    //        D3DXMatrixInverse(&M, 0, &W);
    //    }

    //    {
    //        M *= DefCamera.GetInvViewProjectionMatrix();
    //    }

    //    ScreenRight = Vector3MulMatrix(Vector3(1, 0, Z), M);
    //}

    ////��ʂ̔����̒������v�Z
    //const float ScreenWW = Vector3(Pos - ScreenRight).Length();

    //�X�P�[���䗦���v�Z
    //const float Scale = ScreenWW / 64.0f;

    const float Scale = 0.149960265f * m_Scale;

    //���[���h�ϊ��s����Z�o
    {
        Matrix R;

        D3DXMatrixScaling(T, Scale, Scale, Scale);
        D3DXMatrixRotationY(&R, PI);

        *T *= R;


        T->_41 = Pos.x;
        T->_42 = Pos.y;
        T->_43 = Pos.z;
    }
}

bool BreakScreenObject::Update()
{
    bool isLive = true;
    RATIO Speed = 1.0f;

    ++m_Timer;
    
    if (m_Timer < 30)
    {
        Speed = 0.0f;
    }
    else if (m_Timer < 38)
    {
        Speed = 1.2f;
    }
    else if (m_Timer < 80)
    {
        Speed = 0.0f;
    }
    else if (m_Timer < 110)
    {
        Speed = 2.0f;
        m_Alpha *= 0.85f;

      //  m_MoveY += -0.01f;
      //  m_DiffPosY += m_MoveY;
    }
    else
    {
        isLive = false;
    }

    m_BreakScreenModel.Animation(Speed);
    m_BreakScreenModel.Update();

    CalcBreakScreenWorldMatrix(&m_BreakScreenModel.TransMatrix);

    return isLive;
}

bool BreakScreenObject::Msg(MsgType mt)
{
    return false;
}

void BreakScreenObject::CalcZ()
{
    m_SortZ = 0;
}

void BreakScreenObject::Render()
{

    shader->SetValue("g_Color", D3DXVECTOR4(1, 1, 1, m_Alpha));

    m_BreakScreenModel.Render(
        shader,
        "BreakScreen"
        );
}