#include "RoundUI.h"
#include "../GameSystem/ResourceManager.h"


RoundUI::RoundUI(UINT Round, float Z) :
m_Z(Z),
m_Round(Round),
m_T(0),
m_Count(0)
{
    m_pTexture = DefResource.Get(Resource::TextureType::UI_strings);
}


RoundUI::~RoundUI()
{

}

void RoundUI::CalcZ()
{
    m_SortZ = m_Z;
}

void RoundUI::Render()
{
    m_pTexture->Render(
        300,
        310 + (int)(m_T*-50), 
        550,
        160,

        15,
        265,
        527,
        105,
        RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, m_T)
        );

    RoundRender();
}

bool RoundUI::Update()
{
    const float ApperFrame  = 30.0f;
    const float StopFrame   = 60.0f;
    const float DeleteFrame = 10.0f;

    if (m_Count < ApperFrame)
    {
        m_T = m_Count / ApperFrame;
    }
    else if (m_Count < ApperFrame + StopFrame)
    {
        m_T = 1;
    }
    else
    {
        m_T = 1 - ((m_Count - ApperFrame - StopFrame) / DeleteFrame);
    }

    m_Count += 1.0f;

    return m_Count < ApperFrame + StopFrame + DeleteFrame;
}


void RoundUI::RoundRender()
{
    if (m_Count > 50)
    {
        if (m_Round < 10)
        {
            NumberRender(
                (int)m_Round,
                850,
                200,
                min((m_Count - 50.0f)*0.025f, 1)
                );
        }
        else
        {

            NumberRender(
                (int)(m_Round % 10),
                1050,
                200,
                min((m_Count - 50.0f)*0.025f, 1)
                );

            NumberRender(
                (int)(m_Round / 10),
                850,
                200,
                min((m_Count - 50.0f)*0.025f, 1)
                );
        }
    }
    
}

void RoundUI::NumberRender(int num, int posX, int sizeX, float t)
{
    m_pTexture->Render(
        posX,
        280,
        sizeX,
        160,

        (num % 5) * 140,
        (num / 5) * 130 + 515,
        140,
        120,

        RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, min(1, t*2.0f)*m_T)
        );

    t = min(1, t*1.5f);

    const int UpX = (int)(t * 500);
    const int UpY = (int)(t * 500);

    m_pTexture->Render(
        posX - UpX / 2,
        280 - UpY / 2,
        sizeX + UpX,
        160 + UpY,

        (num % 5) * 140,
        (num / 5) * 100 + 515,
        140,
        100,

        RS_ADD,
        D3DCOLOR_COLORVALUE(1, 1, 1, (1 - t)*0.5f)
        );

}