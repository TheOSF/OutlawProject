#include "FadeGameObject.h"

FadeGameObject::Renderer::Renderer() :
m_Color(0, 0, 0, 0)
{

}


FadeGameObject::Renderer::~Renderer()
{

}

void FadeGameObject::Renderer::CalcZ()
{
    m_SortZ = 0;
}

void FadeGameObject::Renderer::Render()
{
    iexPolygon::Rect(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        RS_COPY,
        m_Color.toDWORD()
        );
}

//---------------------------------------------------------//

FadeGameObject::FadeGameObject(
    COLORf Color,        //色
    UINT   ChangeFrame,  //第一引数の色に変わるまでのフレーム
    UINT   FreezeFrame,  //色が変わってからとまるフレーム
    UINT   BackFrame     //元に戻るフレーム
    ) :
    m_Color(Color),
    m_ChangeFrame(ChangeFrame),
    m_FreezeFrame(FreezeFrame),
    m_BackFrame(BackFrame),
    m_Count(0)
{

}


FadeGameObject::~FadeGameObject()
{

}

bool FadeGameObject::Update()
{
    ++m_Count;

    if (m_Count < m_ChangeFrame)
    {
        const COLORf::RATIO t = (COLORf::RATIO)m_Count / (COLORf::RATIO)m_ChangeFrame;

        m_Renderer.m_Color = COLORf::Lerp(COLORf(0, 0, 0, 0), m_Color, t);

    }
    else if (m_Count < m_ChangeFrame + m_FreezeFrame)
    {
        m_Renderer.m_Color = m_Color;
    }
    else
    {
        const COLORf::RATIO t = (COLORf::RATIO)(m_Count - (m_ChangeFrame + m_FreezeFrame)) / (COLORf::RATIO)m_BackFrame;

        m_Renderer.m_Color = COLORf::Lerp(m_Color, COLORf(0, 0, 0, 0), t);

    }

    return m_Count < m_ChangeFrame + m_FreezeFrame + m_BackFrame;
}

bool FadeGameObject::Msg(MsgType mt)
{
    return false;
}