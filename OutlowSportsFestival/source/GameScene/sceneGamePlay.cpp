#include	"iextreme.h"
#include	"../IexSystem/System.h"
#include	"debug\DebugFunction.h"
#include	"sceneGamePlay.h"
#include	"../Camera/Camera.h"
#include    "../GameSystem/GameObject.h"
#include    "../character/CharacterManager.h"
#include    "../Damage/Damage.h"
#include    "../Render/Renderer.h"
#include    "../Ball/Ball.h"
#include    "../GameSystem/ResourceManager.h"
#include    "../Collision/Collision.h"
#include    "../GameSystem/MatchLightManager.h"

// Bullet
#include "../Library/Bullet/BulletSystem.h"
#include "../../IEX/DebugDrawManager.h"


#include "../GameSystem/GameInitilizer_UsualMatch.h"

#include "../Effect/ParticleManager.h"
#include "../Sound/Sound.h"
#include "../../Input/GamePad/GamePadManager.h"

#include "../Library/Bullet/BulletUpdateGameobject.h"

#include    "../Effect/EffectResourceLoad.h"
#include "../GameSystem/MatchManager.h"
#include "../SceneResult/SceneResult.h"
#include "../IexSystem/Framework.h"
#include "../character/CharacterBase.h"

//*****************************************************************************************************************************
//
//	グローバル変数
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//	初期化
//
//*****************************************************************************************************************************

sceneGamePlay::InitParams::InitParams()
{
    pInitializer = nullptr;
    PlayerArray.fill({ 0, PlayerType::_Player, CharacterType::_Tennis, StrongType::_Usual });
    for (int i = 0; i < 4; ++i)
    {
        PlayerArray.at(i).number = i;
    }

    Round = 1;
    Time = 60 * 60 * 1;
}

sceneGamePlay::sceneGamePlay(
    InitParams& params
    )
{
    m_Params = params;
}

bool sceneGamePlay::Initialize()
{
	

    //	環境設定
    iexLight::SetAmbient(0x404040);
    iexLight::SetFog(800, 1000, 0);

    Vector3 dir(1.0f, -1.0f, -0.5f);
    dir.Normalize();
    iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

    DefCamera.m_Position = Vector3(0, 40, -55);
    DefCamera.m_Target = Vector3(0, 4, 0);

    //当たり判定のデバッグ描画を有効に
    DefDamageMgr.m_DebugDrawVisible = true;


    {// Bullet

        DefBulletSystem.StartUp();
        DefBulletSystem.InitializeBulletPhysics(btVector3(0, -9.8f, 0)*3.5f);

        //更新クラスの作成
        new BulletUpdateGameobject();
    }


    {
        //ＵＩ用文字列画像
        DefResource.Regist(
            Resource::TextureType::UI_strings,
            new iex2DObj("DATA\\UI\\ＵＩ戦闘中文字.png")
            );

        //ＵＩ用プレイヤーネーム
        DefResource.Regist(
            Resource::TextureType::UI_player_name,
            new iex2DObj("DATA\\Texture\\PlayerNo.png")
            );

        //ゲージ
        DefResource.Regist(
            Resource::TextureType::UI_gauges,
            new iex2DObj("DATA\\UI\\ＵＩ体力ゲージ.png")
            );

        //アイコン
        DefResource.Regist(
            Resource::TextureType::UI_icon,
            new iex2DObj("DATA\\UI\\アイコン.png")
            );
    }

    {
        //デバッグ描画用の球メッシュ
        DefResource.Regist(
            Resource::MeshType::Sphere,
            new iexMesh("DATA\\Mesh\\sphere.imo")
            );

        //デバッグ描画用のカプセルメッシュ
        DefResource.Regist(
            Resource::MeshType::Pole,
            new iexMesh("DATA\\Mesh\\Capsure.imo")
            );
    }


    {
        //ボール登録(最終的に登場するキャラクタのボールのみ読み込むようにしたほうが良い)

        DefResource.Regist(
            Resource::MeshType::Amefoot_ball,
            new iexMesh("DATA\\CHR\\Ball\\foot_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::BaseBall_ball,
            new iexMesh("DATA\\CHR\\Ball\\base_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Lacrosse_ball,
            new iexMesh("DATA\\CHR\\Ball\\base_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Soccer_ball,
            new iexMesh("DATA\\CHR\\Ball\\soccer_ball.imo")
            );


        DefResource.Regist(
            Resource::MeshType::Tennis_ball,
            new iexMesh("DATA\\CHR\\Ball\\Tennis_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Tennis_Heart,
			new iexMesh("DATA\\CHR\\Ball\\Tennis_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Volley_ball,
            new iexMesh("DATA\\CHR\\Ball\\soccer_ball.imo")
            );
    }


    {
        //エフェクト読み込み
        EffectResource::Load();
    }

    //GameInitializer_DevelopMode

    m_Params.pInitializer->GameCreate();
    delete m_Params.pInitializer;


	return true;
}

//*****************************************************************************************************************************
//
//		解放
//
//*****************************************************************************************************************************



sceneGamePlay::~sceneGamePlay()
{
	DefGameObjMgr.Release(); //ゲームオブジェクト削除は一番初めに

	DefCharacterMgr.Release();
	DefDamageMgr.Release();
	DefBallMgr.Release();
	DefCamera.Release();
	DefCollisionMgr.Release();
    DefResource.Release();
    DefMatchLightManager.Release();
    DefMatchManager.Release();

    {// Bullet
        DefBulletSystem.ReleaseBulletPhysics();
        DefBulletSystem.ShutDown(true);
    };
}

//*****************************************************************************************************************************
//
//		更新
//
//*****************************************************************************************************************************
void	sceneGamePlay::Update()
{

    //カメラ更新
	DefCamera.Update();

    if (!GetKeyState('M'))
    {
        //全ゲームオブジェクト更新
        DefGameObjMgr.Update();

    }
    
    //キャラクタ押しのける
    DefCharacterMgr.CheckCharacterSpace();
    
    //ダメージのデバッグ描画
    DefDamageMgr.DebugDraw();
    DefDamageMgr.m_DebugDrawVisible = GetKeyState('P') != 0;

    //ライト更新
    DefMatchLightManager.Update();

    //デバッグ用描画切り替え
    if (GetKeyState('L'))
    {
        // Bullet Physics
        DefBulletSystem.DebugDrawWorld();
    }
    
    //シーン移行チェック
    UpdateSceneChange();
}

void sceneGamePlay::UpdateSceneChange()
{
    MatchManager::NextSceneType NextSceneType;

    if (DefMatchManager.GetNextScene(NextSceneType) == false)
    {
        return;
    }


    switch (NextSceneType)
    {
    case MatchManager::NextSceneType::GoResult:
        GoResult();
        break;

    case MatchManager::NextSceneType::ReturnToOption:

        break;

    case MatchManager::NextSceneType::ReturnToTitle:

        break;
    }

}

void sceneGamePlay::GoResult()
{
    //画面テクスチャを作成し、リザルトシーンに渡す
    iex2DObj* pScreenTexture = new iex2DObj((ULONG)iexSystem::ScreenWidth, (ULONG)iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

    //テクスチャに描画
    DefRendererMgr.RenderToTexture(pScreenTexture);

    //パラメータ作成
    SceneResult::ResultStartParam param;

    {
        CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();
        auto it = ChrMap.begin();
        UINT MostHighRound = 0;
        int  Rank = 1;

        while (ChrMap.size() > 0)
        {
            it = ChrMap.begin();

            //もっとも高い勝利数を探す
            while (it != ChrMap.end())
            {
                if (MostHighRound < it->first->m_Params.win)
                {
                    MostHighRound = it->first->m_Params.win;
                }
                ++it;
            }

            it = ChrMap.begin();

            //一番勝利しているキャラクタ
            while (it != ChrMap.end())
            {
                if (MostHighRound == it->first->m_Params.win)
                {
                    param.PlayerList.push_back({ Rank, it->first->m_PlayerInfo.number, it->first->m_PlayerInfo.player_type, it->first->m_PlayerInfo.chr_type });
                    it = ChrMap.erase(it);
                    continue;
                }
                ++it;
            }

            MostHighRound = 0;
            ++Rank;
        }
    }

    //リザルトシーンセット
    MainFrame->ChangeScene(new SceneResult(pScreenTexture, param));
}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

void	sceneGamePlay::Render()
{
    //カメラクリア
    DefCamera.Clear();


    //通常描画
    DefRendererMgr.Render();

}
