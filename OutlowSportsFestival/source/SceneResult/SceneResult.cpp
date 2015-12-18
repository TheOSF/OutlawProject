#include "SceneResult.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"
#include "../Camera/CameraState.h"
#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"
#include "../character/CharacterRenderer.h"
#include "../character/CharacterBase.h"
#include "CharacterResultUpdater.h"
#include "../utillity/StaticGameObjectTemplate.h"
#include "../Render/LightObject.h"
#include "../UI/SelectMenuUI_ControllerPlayer.h"
#include "../SceneOption/SceneOption.h"


SceneResult::SceneResult(
    iex2DObj*                   pScreenTexture,  //スクリーンテクスチャ
    const ResultStartParam&     param
    ):
    m_NextSceneType(NextSceneType::PreSelect),
    m_pStateFunc(&SceneResult::State_Initialize),
    m_Timer(0)
{
    m_pScreenTexture = pScreenTexture;
    m_pBreakScreenModel = nullptr;  /////
    m_StartParam = param;
}

SceneResult::~SceneResult()
{
    DefGameObjMgr.Release(); //ゲームオブジェクト削除は一番初めに
    DefCamera.Release();
    

    delete m_pScreenTexture;
}

void SceneResult::SetNextScene(NextSceneType Next)
{
    m_NextSceneType = Next;
}

void SceneResult::Update()
{
    (this->*m_pStateFunc)();

    DefCamera.Update();
    DefGameObjMgr.Update();

    //シーン分岐
    switch (m_NextSceneType)
    {
    case NextSceneType::PreSelect:

        break;

    case NextSceneType::CharacterSelect:
        MainFrame->ChangeScene(new SceneOption());
        return;

    case NextSceneType::GameOption:
        MainFrame->ChangeScene(new SceneOption());
        return;

    default:
        break;
    }
}

void SceneResult::Render()
{
    DefCamera.Clear();
    DefRendererMgr.Render();
}

//----------------------------------------------------------------//

void SceneResult::State_Initialize()
{
    //画面砕け散るステートへ
    m_pStateFunc = &SceneResult::State_ScreenVanish;

    //カメラを初期位置にセット
    DefCamera.SetNewState(new CameraStateNoWork());

    //キャラクタをセット
    Vector3 pos = Vector3(-4.5f, 0, 0);

    for (auto&it : m_StartParam.PlayerList)
    {
        SetCharacter(pos, it);
        pos.x += 3.0f;
    }

    //スクリーンモデルにテクスチャを張り替える

    //ライトをセット
    SetLights();

}

void SceneResult::State_ScreenVanish()
{
    //カウント更新
    ++m_Timer;

    //スクリーンアニメーション更新

    
    //ボールアニメーション更新


    //時間でキャラクタズームステートへ
    if (m_Timer == 30)
    {
        m_pStateFunc = &SceneResult::State_Back;
        m_Timer = 0;

        DefCamera.m_Position = Vector3(0, 5, -20);
        DefCamera.m_Target = Vector3(0, 5, 0);
    }
}

void SceneResult::State_Back()
{
    //カウント更新
    ++m_Timer;

    //カメラズーム
    if (m_Timer == 1)
    {

    }

    //ボールをキャラクタに向かって飛ばす
    if (m_Timer == 30)
    {

    }

    //WinnerＵＩの表示
    if (m_Timer == 60)
    {

    }

    //メニュー表示ステートへ
    if (m_Timer == 100)
    {
        //メニュー作成
        SelectMenuUI_ControllerPlayer* pController = new SelectMenuUI_ControllerPlayer();

        //全員を操作可能に
        for (int i = 0; i < 4; ++i)
        {
            pController->SetCanControll_ControllerNum((controller::CONTROLLER_NUM)i);
        }
        
        SelectMenuUI* pMenu = new SelectMenuUI(
            pController,
            new iex2DObj("DATA\\RESULT\\ResultSelect.png"), 
            true,
            -1
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::GameOption),
            RectI(0,0,1024,128),
            500,
            80
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::GameOption),
            RectI(0, 128, 880, 128),
            400,
            80
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::GameOption),
            RectI(0, 0, 1024, 128),
            500,
            80
            );
    }
}

void SceneResult::SetLights()
{
    //ライトをセット

    //平行光
    {
        DirLight* pDirLight = new DirLight();

        //夕焼け時のライティングをセット
        pDirLight->param.color = Vector3(0.3f, 0.2f, 0.2f);
        pDirLight->param.vec = Vector3Normalize(Vector3(0.8f, -1, 0.2f));
        pDirLight->param.Shadow.visible = true;
        pDirLight->param.Shadow.Near = 5;
        pDirLight->param.Shadow.Far = 150;
        pDirLight->param.Shadow.origin = pDirLight->param.vec*-50.0f;
        pDirLight->param.Shadow.Size = 120;

        new StaticGameObjectTemplate<DirLight>(pDirLight);
    }

    //環境光
    {
        AmbientLight* pAmbient = new AmbientLight();
        pAmbient->param.color = Vector3(0.37f, 0.26f, 0.26f);
        pAmbient->param.Occlusion.SamplingSize = 0.1f;
        pAmbient->param.Occlusion.Enable = false;

        new StaticGameObjectTemplate<AmbientLight>(pAmbient);
    }

}

void SceneResult::SetCharacter(CrVector3 pos, PlayerInfo& info)
{

    //キャラクタレンダラーの作成
    CharacterRenderer* Renderer = 
        new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(info.ChrType))
            );

    CharacterBase::CreateCharacterModel(Renderer, info.ChrType, info.Num);


    //ワールド変換行列を算出
    {
        const float MulScale = 0.05f;
        Matrix  R;
        Matrix* T = &Renderer->m_TransMatrix;

        D3DXMatrixRotationY(&R, PI);
        D3DXMatrixScaling(T, MulScale, MulScale, MulScale);
        *T *= R;

        T->_41 = pos.x;
        T->_42 = pos.y;
        T->_43 = pos.z;
    }


    //モーションセット
    {
        int(*MotionFunc)(CharacterType::Value);

        if (info.Ranking == 1)
        {
            MotionFunc = &CharacterBase::GetWinMotion;
        }
        else 
        {
            MotionFunc = &CharacterBase::GetLoseMotion;
        }
        
        Renderer->SetMotion(MotionFunc(info.ChrType));
    }

    //更新クラスにセット
    new CharacterResultUpdater(Renderer);

}



//---------------------シーン移行クラス------------------------//

SceneResult::SceneChangeDecide::SceneChangeDecide(SceneResult* Result, NextSceneType Next) :
m_Next(Next),
m_pResult(Result)
{

}


void SceneResult::SceneChangeDecide::Decide()
{
    m_pResult->SetNextScene(m_Next);
}

