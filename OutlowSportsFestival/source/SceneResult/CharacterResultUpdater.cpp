#include "CharacterResultUpdater.h"



//-------------------------------------------------------
//  リザルト中のキャラクタ更新クラス
//-------------------------------------------------------

CharacterResultUpdater::CharacterResultUpdater(CharacterRenderer* pRenderer):
m_pRenderer(pRenderer)
{

}


CharacterResultUpdater::~CharacterResultUpdater()
{
    delete m_pRenderer;
}

bool CharacterResultUpdater::Update()
{
    m_pRenderer->Update(1);

    return true;
}