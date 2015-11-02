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

// Bullet
#include "../Library/Bullet/BulletSystem.h"
#include "../../IEX/DebugDrawManager.h"

#include "../GameSystem/GameInitializer.h"
#include "../GameSystem/GameInitilizer_DevelopMode.h"

#include "../Effect/ParticleManager.h"
#include "../Sound/Sound.h"

// input
#include "../../Input/GamePad/GamePadManager.h"


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
std::vector<InputDevice*> g_InputDevices;


bool sceneGamePlay::Initialize()
{
	
    IGameInitializer* Initializer = new GameInitializer_DevelopMode();

    Initializer->GameCreate();

    delete Initializer;

    //法線・深度バッファを登録
    {
        shader->SetValue("NormalDepthMap",
            DefRendererMgr.GetNormalDepthTexture()->GetTexture());
    }

    //サウンド初期化
    {
        Sound::Initialize();
    }

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
	DefRendererMgr.Release();
	DefDamageMgr.Release();
	DefBallMgr.Release();
	DefCamera.Release();
	DefCollisionMgr.Release();
    DefResource.Release();

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

    //全ゲームオブジェクト更新
    DefGameObjMgr.Update();
    
    //キャラクタ押しのける
    DefCharacterMgr.CheckCharacterSpace();
    
    //ダメージのデバッグ描画
    DefDamageMgr.DebugDraw();
    DefDamageMgr.m_DebugDrawVisible = GetKeyState('P') != 0;

    //デバッグ用描画切り替え
    if (GetKeyState('L'))
    {
        // Bullet Physics
        DefBulletSystem.DebugDrawWorld();
    }


	// ゲームパッド
	GamePadManager::Update();

	if (GamePadManager::GetState(0, GamePadIndex_LeftPad_Up) == 3)
	{
		GamePadManager::Vibration(0, 0, DI_SECONDS*5, 10000);
	}


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
