#include "BreakScreenObject.h"
#include "../Camera/Camera.h"
#include "../GameSystem/GameController.h"

#include "../Effect/GlavityLocus.h"
#include "../Sound/Sound.h"


BreakScreenObject::BreakScreenObject(iex2DObj* pTexture) :
m_BreakScreenModel("DATA\\RESULT\\BreakScreen\\wall_break.iem"),
m_pTexture(pTexture),
m_Timer(0),
m_Scale(1.0f),
m_Alpha(1),
m_MoveY(0),
m_DiffPosY(0)
{

    //位置を初期化
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

    ////画面右端のワールド座標を求める
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

    ////画面の半分の長さを計算
    //const float ScreenWW = Vector3(Pos - ScreenRight).Length();

    //スケール比率を計算
    //const float Scale = ScreenWW / 64.0f;

    const float Scale = 0.149960265f * m_Scale;

    //ワールド変換行列を算出
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
    
    if (m_Timer < 8)
    {
        //カメラのゆれ
        if (m_Timer == 1)
        {
            DefCamera.SetShock(Vector2(0.1f, 0.1f), 5);

            Effect(20);

            //サウンドストップ
            Sound::StopBGM();
        }

        Speed = 1.2f;
    }
    else if (m_Timer < 50)
    {
        Speed = 0.0f;
    }
    else if (m_Timer < 110)
    {
        //カメラのゆれ
        if (m_Timer == 50)
        {
            Effect(20);
            DefCamera.SetShock(Vector2(0.1f, 0.1f), 10);

            Sound::Play(Sound::BGM_Result);
        }

        Speed = 1.0f;
        m_Alpha *= 0.9f;

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

void BreakScreenObject::Effect(int num)
{
    return;////////////////////////////////

    Vector3  color(1.0f, 0.5f, 0);
    Vector3 Nmove = -Vector3AxisZ;
    Vector3 power(0, -0.02f, 0);
    Vector3 move;
    GlavityLocus* g;

    const Vector4
        stCol(color.x, color.y, color.z, 1.0f),
        endCol(color.x, color.y, color.z, 0.5f);

    const Vector4
        stHdCol(color.x, color.y, color.z, 1.0f),
        endHdCol(color.x, color.y, color.z, 0.5f);

    Vector3 pos = m_BreakScreenModel.GetPos();

    for (int i = 0; i < num; ++i)
    {
        move = Vector3Rand() + Nmove;
        move *= frand();
        move *= 0.1f;

        g = new GlavityLocus(
            pos, move, power, 8, 40 + rand() % 20
            );

        g->m_BoundRatio = 0.5f;
        g->m_CheckWall = false;

        g->m_Locus.m_StartParam.Color = stCol;
        g->m_Locus.m_EndParam.Color = endCol;

        g->m_Locus.m_StartParam.HDRColor = stHdCol;
        g->m_Locus.m_EndParam.HDRColor = endHdCol;

        g->m_Locus.m_StartParam.Width = 0.09f;
        g->m_Locus.m_EndParam.Width = 0.00f;

        // g->m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
    }
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