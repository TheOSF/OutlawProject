#include "RoundUI.h"
#include "../GameSystem/ResourceManager.h"


RoundUI::RoundUI(UINT Round, float Z) :
m_Z(Z),
m_Round(Round)
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
        
        );
}