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
#include "../SceneTitle/SceneTitle.h"
#include "../UI/SceneCharacterSelectUI.h"
#include "../utillity/DebugControllGameObject.h"


UINT SceneResult::ResultStartParam::GetPlayerNum()const
{
    UINT ret = 0;

    for (auto it : PlayerList)
    {
        if (it.PlayerType == PlayerType::_Player)
        {
            ret++;
        }
    }

    return ret;
}

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

    delete m_pUITex;
    delete m_pPlayerNumTex;
}

bool SceneResult::Initialize()
{
    //�J�����������ʒu�ɃZ�b�g
    DefCamera.SetNewState(new CameraStateNoWork());

    //�J����������
    GetWinChrCameraParam(DefCamera.m_Position, DefCamera.m_Target);
    DefCamera.Update();

    //��ʃI�u�W�F�N�g�𐶐�
    new BreakScreenObject(m_pScreenTex);

    //�X�e�[�W����
    CreateStage();

    m_pUITex = new iex2DObj("DATA\\RESULT\\ResultSelect.png");
    m_pPlayerNumTex = new iex2DObj("DATA\\RESULT\\Playernumber.png");
    
    {
        iexLight::SetFog(800, 1000, 0);
    }

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

    sceneGamePlay::InitParams Param;

    //�V�[������
    switch (m_NextSceneType)
    {
    case NextSceneType::PreSelect:

        break;

    case NextSceneType::CharacterSelect:
        Param.Round = m_StartParam.Round;
        Param.Time = m_StartParam.RoundFrame;

        MainFrame->ChangeScene(new SceneCharacterSelect(Param, m_StartParam.GetPlayerNum()));
        break;

    case NextSceneType::GameOption:
        MainFrame->ChangeScene(new SceneOption());
        return;

    case NextSceneType::StaffRoll:
        MainFrame->ChangeScene(new SceneTitle());
        return;

    case NextSceneType::Title:
        MainFrame->ChangeScene(new SceneTitle());
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
    for (auto&it : m_StartParam.PlayerList)
    {
        SetCharacter(GetChrPosByNum(it.Num), it);
    }

    

    //�X�N���[�����f���Ƀe�N�X�`���𒣂�ւ���

    //���C�g���Z�b�g
    SetLights();

}

void SceneResult::GetWinChrCameraParam(Vector3& pos, Vector3& target)
{
    Vector3 WinChrPos = GetChrPosByNum(GetWinCharacterPlayerNum());

    WinChrPos.y += 5.0f;

    pos = WinChrPos + Vector3(0, 0, -8);
    target = WinChrPos;

}

int SceneResult::GetWinCharacterPlayerNum()
{
    for (auto& it : m_StartParam.PlayerList)
    {
        if (it.Ranking == 1)
        {
            return (int)it.Num;
        }
    }

    MyAssert(false, "�����L���������ł��܂���ł���");

    return -1;
}

Vector3 SceneResult::GetChrPosByNum(int num)
{
    Vector3 pos = Vector3(-6.0f, 1.95f, 0);
    const float Width = -pos.x / 1.5f;

    pos.x += Width*(float)num;

    return pos;
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

    //Winner�t�h�̕\��
    if (m_Timer == 40)
    {
        SceneCharacterSelectUI* pUI = nullptr;

        {
            //�v���C���[�ԍ�
            pUI = new SceneCharacterSelectUI(
                m_pPlayerNumTex,
                RectI(0, 102 * GetWinCharacterPlayerNum(), 256, 102)
                );

            pUI->m_Target.Color = Vector4(1, 1, 1, 1);
            pUI->m_Target.Pos = Vector2(450, 460);
            pUI->m_Target.Size = Vector2(400, 125);

            pUI->m_Current = pUI->m_Target;
            pUI->m_Current.Color.w = 0.0f;

        }

        {
            //WIN
            pUI = new SceneCharacterSelectUI(
                m_pUITex,
                RectI(500, 0, 524, 250)
                );

            pUI->m_Target.Color = Vector4(1, 1, 1, 1);
            pUI->m_Target.Pos = Vector2(750, 460);
            pUI->m_Target.Size = Vector2(300, 100);

            pUI->m_Current = pUI->m_Target;
            pUI->m_Current.Color.w = 0.0f;

        }
    }

    if (m_Timer == 120)
    {
        DefCamera.SetNewState(new CameraStateMovetoPoint(
            Vector3(0, 5, -12),
            Vector3(0, 5, 0),
            0.05f,
            60,
            CameraStateMovetoPoint::CalcType::_Cos,
            new CameraStateNoWork()
            ));
    }

    //���j���[�\���X�e�[�g��
    if (m_Timer == 300)
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
            m_pUITex, 
            false,
            -1
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::CharacterSelect),
            RectI(0, 0, 500, 100), 
            500,
            100
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::GameOption),
            RectI(0, 100, 500, 100),
            500,
            100
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::Title),
            RectI(0, 200, 500, 100),
            500,
            100
            );

        pMenu->AddMenu(
            new SceneChangeDecide(this, NextSceneType::StaffRoll),
            RectI(0, 300, 500, 100),
            500,
            100
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

    const bool Winchr = info.Ranking == 1;

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
            Vector3 v = Vector3(0, 0, -10) - pos;

            v.Normalize();

            Angle = acosf(fClamp(v.z, 1, -1));
            if (v.x < 0)
            {
                Angle = -Angle;
            }

            if (Winchr)
            {
                Angle = PI;
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

        if (Winchr)
        {
            MotionFunc = &CharacterBase::GetWinMotion;

            //SpotLight* pLight = new SpotLight();

            //pLight->param.color = Vector3(1, 1, 0)*2.0f;

            //pLight->param.origin = pos + Vector3(0, 0, -20);
            //pLight->param.target = pos + Vector3(0, 5, 10);

            //pLight->param.size = 15.0f;
            //pLight->param.Shadow.visible = true;

            //new DebugControllGameObject(
            //    &pLight->param.target,
            //    0,
            //    0.1f,
            //    nullptr,
            //    'Q'
            //    );

            //new DebugControllGameObject(
            //    &pLight->param.origin,
            //    0,
            //    0.1f,
            //    nullptr,
            //    'W'
            //    );

            //new StaticGameObjectTemplate<SpotLight>(pLight);
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

