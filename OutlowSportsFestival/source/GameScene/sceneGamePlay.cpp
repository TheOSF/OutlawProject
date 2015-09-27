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
//	�O���[�o���ϐ�
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//	������
//
//*****************************************************************************************************************************


bool sceneGamePlay::Initialize()
{
	
    IGameInitializer* Initializer = new GameInitializer_DevelopMode();

    Initializer->GameCreate();

    //�@���E�[�x�o�b�t�@��o�^
    {
        shader->SetValue("NormalDepthMap",
            DefRendererMgr.GetNormalDepthTexture()->GetTexture());
    }

    delete Initializer;

	return true;
}

//*****************************************************************************************************************************
//
//		���
//
//*****************************************************************************************************************************

sceneGamePlay::~sceneGamePlay()
{

	DefGameObjMgr.Release(); //�Q�[���I�u�W�F�N�g�폜�͈�ԏ��߂�

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
//		�X�V
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
//		�`��֘A
//
//*****************************************************************************************************************************

void	sceneGamePlay::Render()
{
	DefCamera.Clear();
	
	//�f�o�b�O�p�`��؂�ւ�
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
