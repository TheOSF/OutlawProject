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

#include "../GameSystem/GameInitializer.h"
#include "../GameSystem/GameInitilizer_DevelopMode.h"

#include "../Effect/ParticleManager.h"


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


bool sceneGamePlay::Initialize()
{
	
    IGameInitializer* Initializer = new GameInitializer_DevelopMode();

    Initializer->GameCreate();

    //法線・深度バッファを登録
    {
        shader->SetValue("NormalDepthMap",
            DefRendererMgr.GetNormalDepthTexture()->GetTexture());
    }

    delete Initializer;

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
	{// Bullet Physics

		DefBulletSystem.StepSimulation(1.0f / 60.0f);
	};

	DefCamera.Update();
    
    DefGameObjMgr.Update();
    DefCharacterMgr.CheckCharacterSpace();

    DefDamageMgr.DebugDraw();

    DefDamageMgr.m_DebugDrawVisible = GetKeyState('P') != 0;
}

//*****************************************************************************************************************************
//
//		描画関連
//
//*****************************************************************************************************************************

void	sceneGamePlay::Render()
{
	DefCamera.Clear();
	
	//デバッグ用描画切り替え
	if (GetKeyState('L'))
	{
		// Bullet Physics
		DefBulletSystem.DebugDrawWorld();
	}
	else
	{
        DefRendererMgr.Render();
	}
}
