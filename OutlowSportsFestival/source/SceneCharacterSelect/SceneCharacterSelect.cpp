#include "SceneCharacterSelect.h"
#include "../Camera/Camera.h"
#include	"../IexSystem/System.h"
#include "../IexSystem/Framework.h"
#include "../SceneStageSelect/SceneStageSelect.h"
#include "SelectCharacterWindow.h"
#include "../Render/LightObject.h"
#include "../utillity/StaticGameObjectTemplate.h"

SceneCharacterSelect::SceneCharacterSelect(
    sceneGamePlay::InitParams&  LoadParams,
    UINT                        PlayerNum
    ) :
m_Texture("DATA\\Texture\\particle.png"),
m_pStateFunc(&SceneCharacterSelect::State_PreSelect),
m_Timer(0),
m_PlayerNum(PlayerNum),
m_NextSceneType(NextSceneType::_NoChange),
m_LoadParams(LoadParams)
{
    //	環境設定
    {
        iexLight::SetAmbient(0x404040);
        iexLight::SetFog(800, 1000, 0);

        Vector3 dir(1.0f, -1.0f, -0.5f);
        dir.Normalize();
        iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
    }

    //データ初期化
    InitData();

    //マネージャ
    m_pManager = new CursorManager();

    
    //キャラクタポイントをセット
    SetCharacterPoint();

    //コンピュータ用のポイントをセット
    //SetComputerPoint();

    //ライトセット
    CreateLight();

}

SceneCharacterSelect::~SceneCharacterSelect()
{
    DefGameObjMgr.Release();

    DefCamera.Release();

    delete m_pManager;
}

void SceneCharacterSelect::Update()
{
    static float CY[2] = { 2.0f, 2.0f };

    CY[0] += controller::GetStickValue(controller::stick::right, 0).x*0.05f;
    CY[1] += controller::GetStickValue(controller::stick::right, 0).y*0.05f;

    DefCamera.m_Position = Vector3(0, 2.45f, 0);
    DefCamera.m_Target = Vector3(0, 1.63f, 10);

    DefCamera.Update();
    DefGameObjMgr.Update();

    (this->*m_pStateFunc)();

    switch (m_NextSceneType)
    {
    case NextSceneType::Back:
        
        break;

    case NextSceneType::Next:
        MainFrame->ChangeScene(new SceneStageSelect(m_LoadParams));
        break;

    case NextSceneType::_NoChange:
        break;

    }
}

void SceneCharacterSelect::Render()
{
    DefCamera.Clear();
    
    DefRendererMgr.Render();
}

void SceneCharacterSelect::InitData()
{
    for (UINT i = 0; i < m_LoadParams.PlayerArray.size(); ++i)
    {
        auto& it = m_LoadParams.PlayerArray.at(i);

        it.chr_type = CharacterType::__ErrorType;
        it.number = (int)i;
        it.player_type = PlayerType::_Computer;
        it.strong_type = StrongType::_Usual;
    }
    

    for (UINT i = 0; i < m_PlayerNum; ++i)
    {
        m_LoadParams.PlayerArray[i].player_type = PlayerType::_Player;
    }

}

void SceneCharacterSelect::CreateLight()
{

    //平行光
    DirLight* pDir = new DirLight();

    pDir->param.color = Vector3(0.28f, 0.24f, 0.24f);
    pDir->param.vec = Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f));//Vector3Normalize(Vector3(0.8f, -2, 0.2f));
    pDir->param.Shadow.visible = true;
    pDir->param.Shadow.Near = 5;
    pDir->param.Shadow.Far = 30;
    pDir->param.Shadow.origin = pDir->param.vec*-10.0f + Vector3(0, 0, 10);
    pDir->param.Shadow.Size = 20;

    new StaticGameObjectTemplate<DirLight>(pDir);

    //環境光
    AmbientLight* pAmb = new AmbientLight();

    pAmb->param.color = Vector3(0.29f, 0.29f, 0.29f);
    pAmb->param.Occlusion.SamplingSize = 0.1f;
    pAmb->param.Occlusion.Enable = false;

    new StaticGameObjectTemplate<AmbientLight>(pAmb);
}

void SceneCharacterSelect::SetCharacterPoint()
{
    //キャラのカーソルポイントたち
    {
        SelectPointBase* p;
        Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
        const float Space = 240.0f;

        Vector2 PosA = Vector2(-300, 200);
        Vector2 PosB = Vector2(-100, 150);

        struct
        {
            SelectPointBase::PointType  Type;
            Vector2                     Pos;
        }
        ChrPointData[4] =
        {
            { SelectPointBase::PointType::Tennis, Center + PosA },
            { SelectPointBase::PointType::Soccer, Center + PosB },
            { SelectPointBase::PointType::BaseBall, Center + Vector2(-PosB.x, PosB.y) },
            { SelectPointBase::PointType::AmericanFootBall, Center + Vector2(-PosA.x, PosA.y) },
        };

        for (int i = 0; i < (int)ARRAYSIZE(ChrPointData); ++i)
        {
            p = new SelectPointBase(m_pManager, ChrPointData[i].Type, &m_Texture, RectI(0, 0, 64, 64));
            p->m_Pos = Center;
            p->m_MoveTargetPos = ChrPointData[i].Pos;

            m_ChrPoint[i] = p;
        }

        p = new SelectPointBase(m_pManager, SelectPointBase::PointType::Random, &m_Texture, RectI(0, 0, 64, 64));
        p->m_Pos = Center;
        p->m_MoveTargetPos = Center + Vector2(0, 250);

    }
}


void SceneCharacterSelect::SetOtherComputerCharacter()
{
    struct
    {
        CharacterType::Value Type;
        bool                 Use;
    }
    CharacterData[4]=
    {
        { CharacterType::_Tennis, false },
        { CharacterType::_Soccer, false },
        { CharacterType::_Baseball, false },
        { CharacterType::_Americanfootball, false },
    };

    for (auto& it : m_LoadParams.PlayerArray)
    {
        if (it.chr_type != CharacterType::__ErrorType)
        {
            for (int i = 0; i < (int)ARRAYSIZE(CharacterData); ++i)
            {
                if (CharacterData[i].Type == it.chr_type)
                {
                    CharacterData[i].Use = true;
                    break;
                }
            }
        }
    }

    for (auto& it : m_LoadParams.PlayerArray)
    {
        if (it.chr_type == CharacterType::__ErrorType)
        {
            for (int i = 0; ARRAYSIZE(CharacterData); ++i)
            {
                if (CharacterData[i].Use == false)
                {
                    CharacterData[i].Use = true;
                    it.chr_type = CharacterData[i].Type;
                    break;
                }
            }
        }
    }

}


//-------------------------------------------------------------------//

void SceneCharacterSelect::State_PreSelect()
{
    if (++m_Timer > 60)
    {
        //カーソルをセット
        for (UINT i = 0; i < 4; ++i)
        {
            SelectCursor* p = new SelectCursor(m_pManager, (controller::CONTROLLER_NUM)i, &m_Texture, RectI(0, 0, 128, 128), m_ChrPoint[i]);

            p->m_PlayerInfo = m_LoadParams.PlayerArray.at(i);

            //操作不能に
            if (p->m_PlayerInfo.player_type == PlayerType::_Computer)
            {
                p->m_ControllerNum = controller::__ERROR_CONTROLLER_NUM;
            }

            //キャラクタ表示クラスを作る
            new SelectCharacterWindow(p);
        }

        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Selecting;

        m_Timer = 0;
    }
}

void SceneCharacterSelect::State_Selecting()
{
    //全員選択していたら
    if (m_pManager->isAllPlayerSelected())
    {
        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_FadeOut;

        //情報を取得
        m_pManager->GetData(m_LoadParams);

        //あいているキャラクタデータにコンピュータを入れる
        SetOtherComputerCharacter();
    }
}

void SceneCharacterSelect::State_FadeOut()
{
    if (++m_Timer > 60)
    {
        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Change;

        m_NextSceneType = NextSceneType::Next;
    }
}

void SceneCharacterSelect::State_Change()
{
    
}