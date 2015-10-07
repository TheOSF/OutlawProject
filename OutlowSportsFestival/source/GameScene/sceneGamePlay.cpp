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
#include "../Sound/Sound.h"

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

    delete Initializer;

    //�@���E�[�x�o�b�t�@��o�^
    {
        shader->SetValue("NormalDepthMap",
            DefRendererMgr.GetNormalDepthTexture()->GetTexture());
    }

    //�T�E���h������
    {
        Sound::Initialize();
    }
    

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
    //�J�����X�V
	DefCamera.Update();

    //�S�Q�[���I�u�W�F�N�g�X�V
    DefGameObjMgr.Update();
    
    //�L�����N�^�����̂���
    DefCharacterMgr.CheckCharacterSpace();
    
    //�_���[�W�̃f�o�b�O�`��
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
    //�J�����N���A
	DefCamera.Clear();
	
	//�f�o�b�O�p�`��؂�ւ�
	if (GetKeyState('L'))
	{
		// Bullet Physics
		DefBulletSystem.DebugDrawWorld();
	}
	else
	{
        //�ʏ�`��
        DefRendererMgr.Render();
	}
}
