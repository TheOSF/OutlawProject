#include "GameSetUI.h"
#include "../GameSystem/ResourceManager.h"


GameSetUI::GameSetUI() :
m_Z(0), 
m_T(0),
m_Count(0)
{
    m_pTexture = DefResource.Get(Resource::TextureType::UI_strings);
}


GameSetUI::~GameSetUI()
{

}

void GameSetUI::CalcZ()
{
    m_SortZ = m_Z;
}

void GameSetUI::Render()
{
    m_pTexture->Render(
        300,
        310 + (int)(m_T*-50),
        550,
        160,

        0,
        100*2,
        625,
        100,
        RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, m_T)
        );
}

bool GameSetUI::Update()
{
    const float ApperFrame = 20.0f;
    const float StopFrame = 50.0f;
    const float DeleteFrame = 20.0f;

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
