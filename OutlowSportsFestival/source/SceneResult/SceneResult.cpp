#include "SceneResult.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"


SceneResult::SceneResult(
    iex2DObj*                   pScreenTexture,  //スクリーンテクスチャ
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
    DefGameObjMgr.Release(); //ゲームオブジェクト削除は一番初めに


}

void SceneResult::Update()
{
    (this->*m_pStateFunc)();

    //シーン分岐
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