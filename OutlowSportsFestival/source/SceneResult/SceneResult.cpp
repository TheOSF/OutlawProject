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
#include "../SceneCharacterSelect/SceneCharacterSelect.h"
#include "BreakScreenObject.h"
#include "../Render/MeshRenderer.h"
#include "../Sound/Sound.h"



SceneResult::SceneResult(
    iex2DObj*                   pScreenTexture,  //�X�N���[���e�N�X�`��
    const ResultStartParam&     param
    ):
    m_NextSceneType(NextSceneType::PreSelect),
    m_pStateFunc(&SceneResult::State_Initialize),
    m_Timer(0),
    m_pScreenTex(pScreenTexture)
{

    m_StartParam = param;

    class Pred
    {
    public:
        bool operator ()(const PlayerInfo& p1, const PlayerInfo& p2)
        {
            return (int)p1.Num < (int)p2.Num;
        }
    };

    m_StartParam.PlayerList.sort(Pred());
    
}

SceneResult::~SceneResult()
{
    DefGameObjMgr.Release(); //�Q�[���I�u�W�F�N�g�폜�͈�ԏ��߂�
    DefCamera.Release();
}

bool SceneResult::Initialize()
{
    //�J�����������ʒu�ɃZ�b�g
    DefCamera.SetNewState(new CameraStateNoWork());

    DefCamera.m_Position = Vector3(0, 5, -12);
    DefCamera.m_Target = Vector3(0, 5, 0);

    DefCamera.Update();

    //��ʃI�u�W�F�N�g�𐶐�
    new BreakScreenObject(m_pScreenTex);

    //�X�e�[�W����
    CreateStage();

    return true;
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

    case NextSceneType::GameOption:
        MainFrame->ChangeScene(new SceneOption());
        return;

    case NextSceneType::StaffRoll:
        MainFrame->ChangeScene(new SceneOption());
        return;

    case NextSceneType::Title:
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

    //�L�����N�^���Z�b�g
    Vector3 pos = Vector3(-5.2f, 1.95f, 0);
    const float Width = -pos.x / 1.5f;

    for (auto&it : m_StartParam.PlayerList)
    {
        SetCharacter(pos, it);
        pos.x += Width;
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
    if (m_Timer == 200)
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
            RectI(0,150,1024,128),
            500,
            80
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::Title),
            RectI(41, 470, 240, 110),
            200,
            80
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::StaffRoll),
            RectI(41, 470, 240, 110),
            200,
            80
            );
    }
}

void SceneResult::CreateStage()
{
    MeshRenderer* Renderer = new MeshRenderer(
        new iexMesh("DATA\\RESULT\\Stage\\ResultStage.imo"),
        true,
        MeshRenderer::RenderType::UseColorSpecular
        );

    {
        const Vector3 S = Vector3(0.125f, 0.125f, 0.05f);
        Matrix T, R;

        D3DXMatrixScaling(&T, S.x, S.y, S.z);
        D3DXMatrixRotationY(&R, PI);

        T *= R;

        Renderer->SetMatrix(T);
    }

    new StaticGameObjectTemplate<MeshRenderer>(Renderer);
}

void SceneResult::CreateCharacterBlock(CrVector3 Pos, int Rank)
{
    char Path[MAX_PATH];

    sprintf(Path,"DATA\\RESULT\\RankBlock\\%d.imo", Rank);

    MeshRenderer* pRenderer = new MeshRenderer(
        new iexMesh(Path),
        true,
        MeshRenderer::RenderType::UseColor
        );

    {
        const float Scale = 0.1f;
        Matrix T, R;

        D3DXMatrixScaling(&T, Scale, Scale, Scale);
        D3DXMatrixRotationY(&R, PI);

        T *= R;

        T._41 = Pos.x;
        T._42 = Pos.y;
        T._43 = Pos.z;

        pRenderer->SetMatrix(T);
    }

    new StaticGameObjectTemplate<MeshRenderer>(pRenderer);
    
}

void SceneResult::SetLights()
{
    //���C�g���Z�b�g

    //���s��
    {
        DirLight* pDirLight = new DirLight();

        //�[�Ă����̃��C�e�B���O���Z�b�g
        pDirLight->param.color = Vector3(0.28f, 0.24f, 0.24f);
        pDirLight->param.vec = Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f));
        pDirLight->param.Shadow.visible = true;
        pDirLight->param.Shadow.Near = 5;
        pDirLight->param.Shadow.Far = 30;
        pDirLight->param.Shadow.origin = pDirLight->param.vec*-20.0f;
        pDirLight->param.Shadow.Size = 30;

        new StaticGameObjectTemplate<DirLight>(pDirLight);
    }

    //����
    {
        AmbientLight* pAmbient = new AmbientLight();
        pAmbient->param.color = Vector3(0.29f, 0.29f, 0.29f);
        pAmbient->param.Occlusion.SamplingSize = 0.1f;
        pAmbient->param.Occlusion.Enable = false;

        new StaticGameObjectTemplate<AmbientLight>(pAmbient);
    }

}


void SceneResult::SetCharacter(CrVector3 pos, PlayerInfo& info)
{
    const float RankHeight[] = 
    {
        1.65f,
        1.3f,
        1.1f,
        0.9f,
    };

    //�L�����N�^�����_���[�̍쐬
    CharacterRenderer* Renderer = 
        new CharacterRenderer(
            new BlendAnimationMesh(CharacterBase::GetCharacterModelPath(info.ChrType))
            );

    CharacterBase::CreateCharacterModel(Renderer, info.ChrType, info.Num);


    //�����L���O��̍쐬
    CreateCharacterBlock(pos, info.Ranking);

    //���[���h�ϊ��s����Z�o
    {
        const float MulScale = 0.05f;
        Matrix  R;
        Matrix* T = &Renderer->m_TransMatrix;

        {
            float Angle = 0;
            Vector3 v = DefCamera.m_Position + Vector3(0, 0, -5) - pos;

            v.Normalize();

            Angle = acosf(v.z);
            if (v.x < 0)
            {
                Angle = -Angle;
            }

            D3DXMatrixRotationY(&R, Angle);
            
        }

        D3DXMatrixScaling(T, MulScale, MulScale, MulScale);
        *T *= R;
        
        T->_41 = pos.x;
        T->_42 = pos.y +  RankHeight[info.Ranking - 1];
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

