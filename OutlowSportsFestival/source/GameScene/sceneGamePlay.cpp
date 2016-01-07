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
//	�O���[�o���ϐ�
//
//*****************************************************************************************************************************



//*****************************************************************************************************************************
//
//	������
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
	

    //	���ݒ�
    iexLight::SetAmbient(0x404040);
    iexLight::SetFog(800, 1000, 0);

    Vector3 dir(1.0f, -1.0f, -0.5f);
    dir.Normalize();
    iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

    DefCamera.m_Position = Vector3(0, 40, -55);
    DefCamera.m_Target = Vector3(0, 4, 0);

    //�����蔻��̃f�o�b�O�`���L����
    DefDamageMgr.m_DebugDrawVisible = true;


    {// Bullet

        DefBulletSystem.StartUp();
        DefBulletSystem.InitializeBulletPhysics(btVector3(0, -9.8f, 0)*3.5f);

        //�X�V�N���X�̍쐬
        new BulletUpdateGameobject();
    }


    {
        //�t�h�p������摜
        DefResource.Regist(
            Resource::TextureType::UI_strings,
            new iex2DObj("DATA\\UI\\�t�h�퓬������.png")
            );

        //�t�h�p�v���C���[�l�[��
        DefResource.Regist(
            Resource::TextureType::UI_player_name,
            new iex2DObj("DATA\\Texture\\PlayerNo.png")
            );

        //�Q�[�W
        DefResource.Regist(
            Resource::TextureType::UI_gauges,
            new iex2DObj("DATA\\UI\\�t�h�̗̓Q�[�W.png")
            );

        //�A�C�R��
        DefResource.Regist(
            Resource::TextureType::UI_icon,
            new iex2DObj("DATA\\UI\\�A�C�R��.png")
            );
    }

    {
        //�f�o�b�O�`��p�̋����b�V��
        DefResource.Regist(
            Resource::MeshType::Sphere,
            new iexMesh("DATA\\Mesh\\sphere.imo")
            );

        //�f�o�b�O�`��p�̃J�v�Z�����b�V��
        DefResource.Regist(
            Resource::MeshType::Pole,
            new iexMesh("DATA\\Mesh\\Capsure.imo")
            );
    }


    {
        //�{�[���o�^(�ŏI�I�ɓo�ꂷ��L�����N�^�̃{�[���̂ݓǂݍ��ނ悤�ɂ����ق����ǂ�)

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
        //�G�t�F�N�g�ǂݍ���
        EffectResource::Load();
    }

    //GameInitializer_DevelopMode

    m_Params.pInitializer->GameCreate();
    delete m_Params.pInitializer;


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
//		�X�V
//
//*****************************************************************************************************************************
void	sceneGamePlay::Update()
{

    //�J�����X�V
	DefCamera.Update();

    if (!GetKeyState('M'))
    {
        //�S�Q�[���I�u�W�F�N�g�X�V
        DefGameObjMgr.Update();

    }
    
    //�L�����N�^�����̂���
    DefCharacterMgr.CheckCharacterSpace();
    
    //�_���[�W�̃f�o�b�O�`��
    DefDamageMgr.DebugDraw();
    DefDamageMgr.m_DebugDrawVisible = GetKeyState('P') != 0;

    //���C�g�X�V
    DefMatchLightManager.Update();

    //�f�o�b�O�p�`��؂�ւ�
    if (GetKeyState('L'))
    {
        // Bullet Physics
        DefBulletSystem.DebugDrawWorld();
    }
    
    //�V�[���ڍs�`�F�b�N
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
    //��ʃe�N�X�`�����쐬���A���U���g�V�[���ɓn��
    iex2DObj* pScreenTexture = new iex2DObj((ULONG)iexSystem::ScreenWidth, (ULONG)iexSystem::ScreenHeight, IEX2D_RENDERTARGET);

    //�e�N�X�`���ɕ`��
    DefRendererMgr.RenderToTexture(pScreenTexture);

    //�p�����[�^�쐬
    SceneResult::ResultStartParam param;

    {
        CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();
        auto it = ChrMap.begin();
        UINT MostHighRound = 0;
        int  Rank = 1;

        while (ChrMap.size() > 0)
        {
            it = ChrMap.begin();

            //�����Ƃ�������������T��
            while (it != ChrMap.end())
            {
                if (MostHighRound < it->first->m_Params.win)
                {
                    MostHighRound = it->first->m_Params.win;
                }
                ++it;
            }

            it = ChrMap.begin();

            //��ԏ������Ă���L�����N�^
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

    //���U���g�V�[���Z�b�g
    MainFrame->ChangeScene(new SceneResult(pScreenTexture, param));
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


    //�ʏ�`��
    DefRendererMgr.Render();

}
