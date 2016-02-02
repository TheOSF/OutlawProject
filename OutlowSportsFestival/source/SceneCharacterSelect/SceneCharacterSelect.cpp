#include "SceneCharacterSelect.h"
#include "../Camera/Camera.h"
#include	"../IexSystem/System.h"
#include "../IexSystem/Framework.h"
#include "../SceneStageSelect/SceneStageSelect.h"
#include "SelectCharacterWindow.h"
#include "../Render/LightObject.h"
#include "../utillity/StaticGameObjectTemplate.h"
#include "../Render/MeshRenderer.h"
#include "../GameSystem/ResourceManager.h"
#include "../Sound/Sound.h"
#include "../SceneOption/SceneOption.h"
#include "UI/SceneCharacterSelectUI.h"

SceneCharacterSelect::SceneCharacterSelect(
    sceneGamePlay::InitParams&  LoadParams,
    UINT                        PlayerNum
    ) :
    m_Texture("DATA\\ChrSelect\\character_select.png"),
    m_pStateFunc(&SceneCharacterSelect::State_PreSelect),
    m_Timer(0),
    m_PlayerNum(PlayerNum),
    m_NextSceneType(NextSceneType::_NoChange),
    m_LoadParams(LoadParams),
    m_BackTex("DATA\\Texture\\mizu.png"),
    m_OK_Tex("DATA\\ChrSelect\\OK\\OK.png"),
    m_pOK_UI(nullptr)
{

}

SceneCharacterSelect::~SceneCharacterSelect()
{
    DefGameObjMgr.Release();

    DefCamera.Release();
}

bool SceneCharacterSelect::Initialize()
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

    //コンピュータカーソルのデフォルト位置を生成
    m_pComputerDefaultPoint = new SelectPointBase(m_pManager, SelectPointBase::PointType::ComputerDefaultPoint, &m_Texture, RectI(0, 0, 128, 128));
    m_pComputerDefaultPoint->m_MoveTargetPos = Vector2((float)iexSystem::ScreenWidth / 2, -500);
    m_pComputerDefaultPoint->m_Pos = m_pComputerDefaultPoint->m_MoveTargetPos;

    //キャラクタポイントをセット
    SetCharacterPoint();

    // カーソルの生成
    SetCursor();

    // シーンUIをセット
    SetSceneUI();

    //コンピュータ用のポイントをセット
    SetComputerPoint();

    SetComputerStrongPoint();

    //ライトセット
    CreateLight();

    //床セット
    CreateFloor();


    //背景オブジェクトを生成
    CreateBack();


    Sound::StopBGM();
    Sound::Play(Sound::BGM_ChrSelect, 1, true);

    return true;
}

void SceneCharacterSelect::Update()
{
    DefCamera.m_Position = Vector3(0, 2.45f, 0);
    DefCamera.m_Target = Vector3(0, 1.98f, 10);

    DefCamera.Update();

    (this->*m_pStateFunc)();

    DefGameObjMgr.Update();

    switch ( m_NextSceneType )
    {
    case NextSceneType::Back:
        MainFrame->ChangeScene(new SceneOption());
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
    m_Cursor.fill(nullptr);

    for ( UINT i = 0; i < m_LoadParams.PlayerArray.size(); ++i )
    {
        auto& it = m_LoadParams.PlayerArray.at(i);

        it.chr_type = CharacterType::__ErrorType;
        it.number = (int)i;
        it.player_type = PlayerType::_Computer;
        it.strong_type = StrongType::_Usual;
    }


    for ( UINT i = 0; i < m_PlayerNum; ++i )
    {
        m_LoadParams.PlayerArray[i].player_type = PlayerType::_Player;
    }

}

void SceneCharacterSelect::CreateLight()
{

    //平行光
    DirLight* pDir = new DirLight();

    pDir->param.color = Vector3(0.28f, 0.24f, 0.24f);
    pDir->param.vec = Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f)); //Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f));
    pDir->param.Shadow.visible = true;
    pDir->param.Shadow.Near = 5;
    pDir->param.Shadow.Far = 30;
    pDir->param.Shadow.origin = pDir->param.vec*-10.0f + Vector3(0, 0, 10);
    pDir->param.Shadow.Size = 17;

    new StaticGameObjectTemplate<DirLight>(pDir);

    //環境光
    AmbientLight* pAmb = new AmbientLight();

    pAmb->param.color = Vector3(0.29f, 0.29f, 0.29f);
    pAmb->param.Occlusion.SamplingSize = 0.1f;
    pAmb->param.Occlusion.Enable = false;

    new StaticGameObjectTemplate<AmbientLight>(pAmb);
}

void SceneCharacterSelect::CreateFloor()
{

    MeshRenderer* pRenderer = new MeshRenderer(
        new IEXMESH("DATA\\ChrSelect\\yuka\\yuka.imo"),
        true,
        MeshRenderer::RenderType::UseColor_DarkToAlpha
        );
    {
        Matrix T, R;

        const float Scale = 3.8f;

        D3DXMatrixScaling(&T, Scale, Scale, Scale);
        D3DXMatrixRotationY(&R, PI);

        T *= R;

        T._43 = 4.5f;

        pRenderer->SetMatrix(T);
    }

    pRenderer->m_Lighting = Vector3(1, 1, 1)*0.12f;

    new StaticGameObjectTemplate<MeshRenderer>(pRenderer);

}

void SceneCharacterSelect::CreateBack()
{

    class Renderer :public ForwardRenderer {
        iex2DObj* pTexture;
        float Z;
    public:
        Renderer(
            iex2DObj* pTexture
            ) :
            pTexture(pTexture),
            Z(1)
        {}

        void CalcZ()
        {
            m_SortZ = Z;
        }

        //描画(自動的に呼ばれる)
        void Render()
        {
            pTexture->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1024, 512, RS_COPY, 0xFFFFFFFF, Z);
        }

    };

    new StaticGameObjectTemplate<Renderer>(new Renderer(&m_BackTex));

}

void SceneCharacterSelect::SetSceneUI()
{
    const Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
    
    {
        SceneCharacterSelectUI* p = new SceneCharacterSelectUI(&m_Texture, RectI(0, 732, 731, 95));

        p->m_Current.Size = p->m_Target.Size = Vector2(731, 95);

        p->m_Target.Pos = p->m_Current.Size*0.5f;
        p->m_Current.Pos = p->m_Target.Pos - Vector2(0, -100);

        p->m_Z = -1;
    }

    {
        m_pOK_UI = new SceneCharacterSelectUI(&m_OK_Tex, RectI(0, 0, 512, 512));
        m_pOK_UI->m_Current.Pos = m_pOK_UI->m_Target.Pos = Center;
        m_pOK_UI->m_Current.Size = Vector2(0, 0);
        m_pOK_UI->m_Current.Color = Vector4(0, 0, 0, 0);
        m_pOK_UI->m_SortZ = 0.0f;
        m_pOK_UI->m_Z = -5;
    }

    {
        SceneCharacterSelectUI* p = new SceneCharacterSelectUI(&m_Texture, RectI(0, 827, 308, 102));

        p->m_Current.Size = p->m_Target.Size = Vector2(308, 102);

        p->m_Target.Pos = Vector2(p->m_Current.Size.x*0.5f, (float)iexSystem::ScreenHeight - p->m_Target.Size.y*0.5f);
        p->m_Current.Pos = p->m_Target.Pos - Vector2(0, -1000);

        p->m_Z = -1;
    }
}

void SceneCharacterSelect::SetComputerPoint()
{
    Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
    const float OffsetY = -100;

    Vector2 ComPoints[4] = {
        Center + Vector2(-420,OffsetY),
        Center + Vector2(-160,OffsetY),
        Center + Vector2(160,OffsetY),
        Center + Vector2(420,OffsetY)
    };

    for ( UINT i = 0; i < m_Cursor.size(); i++ )
    {
        SelectCursor* pCursor = m_Cursor.at(i);
        if ( pCursor->m_PlayerInfo.player_type != PlayerType::_Computer )continue;

        SelectPointBase* p =
            new SelectPointBase(m_pManager, SelectPointBase::PointType::ComputerChrSelect, nullptr, RectI(0, 0, 128, 128), pCursor, m_ChrPoint.at(i));
        p->m_Pos = Center;
        p->m_MoveTargetPos = ComPoints[i];
    }
}

void SceneCharacterSelect::SetComputerStrongPoint()
{
    Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
    const float OffsetY = 70;

    Vector2 Points[4] = {
        Center + Vector2(-420,OffsetY),
        Center + Vector2(-160,OffsetY),
        Center + Vector2(160,OffsetY),
        Center + Vector2(420,OffsetY)
    };

    for ( UINT i = 0; i < m_Cursor.size(); i++ )
    {
        SelectCursor* pCursor = m_Cursor.at(i);
        if ( pCursor->m_PlayerInfo.player_type != PlayerType::_Computer )continue;

        SelectPointBase* p =
            new SelectPointBase(m_pManager, SelectPointBase::PointType::ComputerStrong, &m_Texture, RectI(0, 384, 128, 64), pCursor, m_ChrPoint.at(i));
        p->m_Pos = Center;
        p->m_MoveTargetPos = Points[i];
        p->m_Size = Vector2(120, 60);
    }
}

void SceneCharacterSelect::SetCharacterPoint()
{
    //キャラのカーソルポイントたち
    {
        SelectPointBase* p;
        Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
        const float Space = 240.0f;

        Vector2 PosA = Vector2(-300, 230);
        Vector2 PosB = Vector2(-100, 180);

        struct {
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

        for ( int i = 0; i < (int)ARRAYSIZE(ChrPointData); ++i )
        {
            p = new SelectPointBase(m_pManager, ChrPointData[i].Type, &m_Texture, RectI(128 * i, 0, 128, 128));
            p->m_Pos = Center;
            p->m_MoveTargetPos = ChrPointData[i].Pos;

            m_ChrPoint[i] = p;
        }

        p = new SelectPointBase(m_pManager, SelectPointBase::PointType::Random, &m_Texture, RectI(512, 0, 128, 128));
        p->m_Pos = Center;
        p->m_MoveTargetPos = Center + Vector2(0, 280);
    }
}


void SceneCharacterSelect::SetOtherComputerCharacter()
{
    struct {
        CharacterType::Value Type;
        bool                 Use;
    }
    CharacterData[4] =
    {
        { CharacterType::_Tennis, false },
        { CharacterType::_Soccer, false },
        { CharacterType::_Baseball, false },
        { CharacterType::_Americanfootball, false },
    };

    // 使われているキャラクタを特定
    for ( auto& it : m_LoadParams.PlayerArray )
    {
        if ( it.chr_type != CharacterType::__ErrorType )
        {
            for ( int i = 0; i < (int)ARRAYSIZE(CharacterData); ++i )
            {
                if ( CharacterData[i].Type == it.chr_type )
                {
                    CharacterData[i].Use = true;
                    break;
                }
            }
        }
    }

    for ( auto& it : m_LoadParams.PlayerArray )
    {
        if ( it.chr_type == CharacterType::__ErrorType )
        {
            for ( int i = 0; ARRAYSIZE(CharacterData); ++i )
            {
                if ( CharacterData[i].Use == false )
                {
                    CharacterData[i].Use = true;
                    it.chr_type = CharacterData[i].Type;
                    break;
                }
            }
        }
    }

}

void SceneCharacterSelect::SetCursor()
{
    for ( UINT i = 0; i < m_Cursor.size(); ++i )
    {
        SelectPointBase* pDefault = m_ChrPoint.at(i);
        if ( m_LoadParams.PlayerArray.at(i).player_type == PlayerType::_Computer )
        {
            pDefault = m_pComputerDefaultPoint;
        }

        SelectCursor* p = new SelectCursor(
            m_pManager,
            (controller::CONTROLLER_NUM)i,
            &m_Texture,
            RectI(256 * i , 128, 256, 256),
            m_pComputerDefaultPoint,
            pDefault);

        p->m_PlayerInfo = m_LoadParams.PlayerArray.at(i);

        {
            const CharacterType::Value Type = p->m_PlayerInfo.chr_type;

            p->m_PlayerInfo = m_LoadParams.PlayerArray.at(i);
            p->m_PlayerInfo.chr_type = Type;

            //操作不能に
            if ( p->m_PlayerInfo.player_type == PlayerType::_Computer )
            {
                p->m_PlayerInfo.chr_type = CharacterType::__ErrorType;
                p->m_ControllerNum = controller::__ERROR_CONTROLLER_NUM;
            }
        }
        m_Cursor.at(i) = p;
    }
}

//-------------------------------------------------------------------//

void SceneCharacterSelect::State_PreSelect()
{
    if ( ++m_Timer > 60 )
    {
        //カーソルをセット
        for ( UINT i = 0; i < m_Cursor.size(); ++i )
        {
            SelectCursor* p = m_Cursor.at(i);
            SceneCharacterSelectUI* pComUI = nullptr;

            if ( p->m_PlayerInfo.player_type != PlayerType::_Computer )
            {
                p->SetPoint(m_ChrPoint.at(i));
                p->m_Lock = false;
            }
            //else
            //{
            //    Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
            //    const float OffsetY = -100;

            //    Vector2 ComPoints[4] = {
            //        Center + Vector2(-420,OffsetY),
            //        Center + Vector2(-160,OffsetY),
            //        Center + Vector2(160,OffsetY),
            //        Center + Vector2(420,OffsetY)
            //    };

            //    pComUI = new SceneCharacterSelectUI(&m_Texture, RectI(0, 0, 128, 128));
            //    Vector2 EndOffset(0, -(float)iexSystem::ScreenHeight * 0.43f);
            //    pComUI->m_Current.Pos = Center;
            //    pComUI->m_Target.Pos = ComPoints[i];
            //    pComUI->m_Current.Size = pComUI->m_Target.Size = Vector2(120, 120);
            //    pComUI->m_Visible = false;
            //}
            //キャラクタ表示クラスを作る
            new SelectCharacterWindow(p, &m_Texture);
        }

        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Selecting;

        m_Timer = 0;
    }
}

void SceneCharacterSelect::State_Selecting()
{
    //全員選択していたら
    if ( m_pManager->isAllPlayerSelected() )
    {

        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Confirm;
        m_Timer = 0;

        //情報を取得
        m_pManager->GetData(m_LoadParams);

        //あいているキャラクタデータにコンピュータを入れる
        SetOtherComputerCharacter();
    }
    else if ( !m_pManager->isLocked(0) )
    {
        if ( controller::GetTRG(controller::button::batu, 0) )
        {

            Sound::Play(Sound::Cursor_cancel);

            //ステート移行
            m_pStateFunc = &SceneCharacterSelect::State_BackToOption;
            m_Timer = 0;
        }
    }
}

void SceneCharacterSelect::State_Confirm()
{
    if ( m_Timer == 0 )
    {
        // UI 表示
        m_pOK_UI->m_Target.Size = Vector2(500, 500);
        m_pOK_UI->m_Current.Color = Vector4(1, 1, 1, 1);
    }

    if ( ++m_Timer >= 5 )
    {
        bool is_canceled = false;
        bool is_pressed[4] = { false };

        for ( size_t i = 0; i < m_Cursor.size(); i++ )
        {

            // 一応コンピュータは処理しないようにしておく
            if ( m_Cursor.at(i)->m_PlayerInfo.player_type == PlayerType::_Computer )
            {
                continue;
            }

            is_pressed[i] = controller::GetTRG(controller::button::batu, (controller::CONTROLLER_NUM)i);
            if ( is_pressed[i] )
            {
                m_Cursor.at(i)->m_Selected = false;
                m_Cursor.at(i)->m_Lock = false;
                is_canceled = true;
            }
        }

        if ( is_canceled )
        {
            //ステート移行
            m_pStateFunc = &SceneCharacterSelect::State_Selecting;
            m_Timer = 0;

            // UI 表示
            m_pOK_UI->m_Target.Size = Vector2(0, 0);
            m_pOK_UI->m_Current.Color = Vector4(0, 0, 0, 0);

            Sound::Play(Sound::Cursor_cancel);
        }
        else if ( controller::GetTRGAnyController(controller::button::maru) )
        {
            //ステート移行
            m_pStateFunc = &SceneCharacterSelect::State_FadeOut;
            m_Timer = 0;

            Sound::Play(Sound::Scene_Enter);
        }
    }
}

void SceneCharacterSelect::State_FadeOut()
{

    if ( m_Timer == 0 )
    {
        // 操作不能に
        for ( auto&& it : m_Cursor )
        {
            it->m_ControllerNum = controller::__ERROR_CONTROLLER_NUM;
        }
    }

    if ( ++m_Timer > 10 )
    {
        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Change;

        m_NextSceneType = NextSceneType::Next;

        m_Timer = 0;
    }
}

void SceneCharacterSelect::State_BackToOption()
{

    if ( m_Timer == 0 )
    {
        // 操作不能に
        for ( auto&& it : m_Cursor )
        {
            it->m_ControllerNum = controller::__ERROR_CONTROLLER_NUM;
        }
    }

    if ( ++m_Timer > 2 )
    {
        //ステート移行
        m_pStateFunc = &SceneCharacterSelect::State_Change;

        m_NextSceneType = NextSceneType::Back;

        m_Timer = 0;
    }

}

void SceneCharacterSelect::State_Change()
{

}