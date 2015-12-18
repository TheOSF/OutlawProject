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
    iex2DObj*                   pScreenTexture,  //�X�N���[���e�N�X�`��
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
    DefGameObjMgr.Release(); //�Q�[���I�u�W�F�N�g�폜�͈�ԏ��߂�
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

    //�V�[������
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
    //��ʍӂ��U��X�e�[�g��
    m_pStateFunc = &SceneResult::State_ScreenVanish;

    //�J�����������ʒu�ɃZ�b�g
    DefCamera.SetNewState(new CameraStateNoWork());

    //�L�����N�^���Z�b�g
    Vector3 pos = Vector3(-4.5f, 0, 0);

    for (auto&it : m_StartParam.PlayerList)
    {
        SetCharacter(pos, it);
        pos.x += 3.0f;
    }

    //�X�N���[�����f���Ƀe�N�X�`���𒣂�ւ���

    //���C�g���Z�b�g
    SetLights();

}

void SceneResult::State_ScreenVanish()
{
    //�J�E���g�X�V
    ++m_Timer;

    //�X�N���[���A�j���[�V�����X�V

    
    //�{�[���A�j���[�V�����X�V


    //���ԂŃL�����N�^�Y�[���X�e�[�g��
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
    //�J�E���g�X�V
    ++m_Timer;

    //�J�����Y�[��
    if (m_Timer == 1)
    {

    }

    //�{�[�����L�����N�^�Ɍ������Ĕ�΂�
    if (m_Timer == 30)
    {

    }

    //Winner�t�h�̕\��
    if (m_Timer == 60)
    {

    }

    //���j���[�\���X�e�[�g��
    if (m_Timer == 100)
    {
        //���j���[�쐬
        SelectMenuUI_ControllerPlayer* pController = new SelectMenuUI_ControllerPlayer();

        //�S���𑀍�\��
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
    //���C�g���Z�b�g

    //���s��
    {
        DirLight* pDirLight = new DirLight();

        //�[�Ă����̃��C�e�B���O���Z�b�g
        pDirLight->param.color = Vector3(0.3f, 0.2f, 0.2f);
        pDirLight->param.vec = Vector3Normalize(Vector3(0.8f, -1, 0.2f));
        pDirLight->param.Shadow.visible = true;
        pDirLight->param.Shadow.Near = 5;
        pDirLight->param.Shadow.Far = 150;
        pDirLight->param.Shadow.origin = pDirLight->param.vec*-50.0f;
        pDirLight->param.Shadow.Size = 120;

        new StaticGameObjectTemplate<DirLight>(pDirLight);
    }

    //����
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

    //�L�����N�^�����_���[�̍쐬
    CharacterRenderer* Renderer = 
        new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(info.ChrType))
            );

    CharacterBase::CreateCharacterModel(Renderer, info.ChrType, info.Num);


    //���[���h�ϊ��s����Z�o
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


    //���[�V�����Z�b�g
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

    //�X�V�N���X�ɃZ�b�g
    new CharacterResultUpdater(Renderer);

}



//---------------------�V�[���ڍs�N���X------------------------//

SceneResult::SceneChangeDecide::SceneChangeDecide(SceneResult* Result, NextSceneType Next) :
m_Next(Next),
m_pResult(Result)
{

}


void SceneResult::SceneChangeDecide::Decide()
{
    m_pResult->SetNextScene(m_Next);
}

