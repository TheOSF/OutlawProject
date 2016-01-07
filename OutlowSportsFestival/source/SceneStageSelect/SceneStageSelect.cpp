#include "SceneStageSelect.h"
#include "../Camera/Camera.h"
#include	"../IexSystem/System.h"
#include "../IexSystem/Framework.h"
#include"../GameSystem/GameObject.h"
#include "../Render/Renderer.h"


#include "../GameSystem/GameInitilizer_UsualMatch.h"

SceneStageSelect::SceneStageSelect(
    sceneGamePlay::InitParams&  LoadParams
    ) :
    m_pStateFunc(&SceneStageSelect::State_PreSelect),
    m_Timer(0),
    m_NextSceneType(NextSceneType::_NoChange),
    m_LoadParams(LoadParams)
{
    //	ŠÂ‹«Ý’è
    {
        iexLight::SetAmbient(0x404040);
        iexLight::SetFog(800, 1000, 0);

        Vector3 dir(1.0f, -1.0f, -0.5f);
        dir.Normalize();
        iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
    }



}

SceneStageSelect::~SceneStageSelect()
{
    DefGameObjMgr.Release();

    DefCamera.Release();
}

void SceneStageSelect::Update()
{
    DefCamera.Update();
    DefGameObjMgr.Update();

    (this->*m_pStateFunc)();

    switch (m_NextSceneType)
    {
    case NextSceneType::Back:

        break;

    case NextSceneType::Next:
        MainFrame->ChangeScene(new sceneGamePlay(m_LoadParams));
        break;

    case NextSceneType::_NoChange:
        break;

    }
}

void SceneStageSelect::Render()
{
    DefCamera.Clear();

    DefRendererMgr.Render();
}



//-------------------------------------------------------------------//

void SceneStageSelect::State_PreSelect()
{
    GameInitializer_UsualMatch::StageType Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
    const UINT Round = 2;
    const UINT Frame = 60 * 60 * 2;

    m_LoadParams.pInitializer = new GameInitializer_UsualMatch(Stage, m_LoadParams);

    m_pStateFunc = &SceneStageSelect::State_Selecting;
}

void SceneStageSelect::State_Selecting()
{
    m_NextSceneType = NextSceneType::Next;
}

void SceneStageSelect::State_FadeOut()
{

}

void SceneStageSelect::State_Change()
{

}