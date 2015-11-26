#include "SceneResult.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"


SceneResult::SceneResult(
    iex2DObj*                   pScreenTexture,  //�X�N���[���e�N�X�`��
    const ResultStartParam&     param
    ):
    m_NextSceneType(NextSceneType::CharacterSelect)
{
    m_pScreenTexture = pScreenTexture;
    m_pBreakScreenModel = nullptr;  /////
    m_StartParam = param;
}

SceneResult::~SceneResult()
{
    DefGameObjMgr.Release(); //�Q�[���I�u�W�F�N�g�폜�͈�ԏ��߂�


}

void SceneResult::Update()
{
    (this->*m_pStateFunc)();

    //�V�[������
    switch (m_NextSceneType)
    {
    case NextSceneType::PreSelect:

        return;

    case NextSceneType::CharacterSelect:

        return;

    default:
        break;
    }
}

void SceneResult::Render()
{

}