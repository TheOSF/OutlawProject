#include "CharacterResultUpdater.h"



//-------------------------------------------------------
//  ���U���g���̃L�����N�^�X�V�N���X
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