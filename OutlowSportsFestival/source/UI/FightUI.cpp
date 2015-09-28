#include "FightUI.h"
#include "../GameSystem/ResourceManager.h"


FightUI::FightUI(float Z) :
m_Z(Z),
m_Count(0),
m_Delete(false)
{
    m_pTexture = DefResource.Get(Resource::TextureType::UI_strings);
}


FightUI::~FightUI()
{

}

void FightUI::CalcZ()
{
    m_SortZ = m_Z;
}

void FightUI::Render()
{
    const float ApperFrame = 10.0f;
    const float EndFrame = 35.0f;
    const int SizeX = 480;
    const int SizeY = 150;

    if (ApperFrame < m_Count)
    {
        const float t = 1.0f + (m_Count - ApperFrame)*0.1f;
        float alpha;

        alpha = 1 - (m_Count - ApperFrame) / (EndFrame);
        alpha = max(alpha, 0);

        FightRender((int)((float)SizeX*t), (int)((float)SizeY*t), RS_COPY, alpha, 0);
        FightRender((int)((float)SizeX*t), (int)((float)SizeY*t), RS_ADD, alpha, 0);

        if (m_Count > EndFrame + ApperFrame)
        {
            m_Delete = true;
        }
    }
    else
    {
        FightRender(SizeX, SizeY, RS_COPY, m_Count / ApperFrame, (int)(100 * (1-(m_Count / ApperFrame))));
    }
}

bool FightUI::Update()
{
    m_Count += 1.0f;

    return !m_Delete;
}

void FightUI::FightRender(
    int    SizeX,
    int    SizeY,
    DWORD  dw_flg,
    RATIO  alpha,
    int    diffPosY
    )
{
    m_pTexture->Render(
        640 - SizeX / 2,
        360 - SizeY / 2 + diffPosY,
        SizeX,
        SizeY,

        15,
        377,
        560,
        108,

        dw_flg,
        D3DCOLOR_COLORVALUE(1, 1, 1, alpha)
        );
}