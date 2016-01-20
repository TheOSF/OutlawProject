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
m_BackTex("DATA\\Texture\\mizu.png")
{
    //	���ݒ�
    {
        iexLight::SetAmbient(0x404040);
        iexLight::SetFog(800, 1000, 0);

        Vector3 dir(1.0f, -1.0f, -0.5f);
        dir.Normalize();
        iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
    }

    //�f�[�^������
    InitData();

    //�}�l�[�W��
    m_pManager = new CursorManager();

    //�R���s���[�^�J�[�\���̃f�t�H���g�ʒu�𐶐�
    m_pComputerDefaultPoint = new SelectPointBase(m_pManager, SelectPointBase::PointType::ComputerDefaultPoint, &m_Texture, RectI(0, 0, 128, 128));
    m_pComputerDefaultPoint->m_MoveTargetPos = Vector2((float)iexSystem::ScreenWidth / 2, -500);
    m_pComputerDefaultPoint->m_Pos = m_pComputerDefaultPoint->m_MoveTargetPos;


    //�@�싅�֘A
    DefResource.Regist(
        Resource::MeshType::Bat,
        new iexMesh("DATA\\CHR\\SanoBaseball\\bat.imo")
        );

    //�L�����N�^�|�C���g���Z�b�g
    SetCharacterPoint();

    //�R���s���[�^�p�̃|�C���g���Z�b�g
    //SetComputerPoint();

    //���C�g�Z�b�g
    CreateLight();

    //���Z�b�g
    CreateFloor();


    //�w�i�I�u�W�F�N�g�𐶐�
    CreateBack();


    Sound::StopBGM();
    Sound::Play(Sound::BGM_ChrSelect, 1, true);
}

SceneCharacterSelect::~SceneCharacterSelect()
{
    DefGameObjMgr.Release();


    DefResource.Release();
    DefCamera.Release();

    delete m_pManager;
}

void SceneCharacterSelect::Update()
{
    DefCamera.m_Position = Vector3(0, 2.45f, 0);
    DefCamera.m_Target = Vector3(0, 1.63f, 10);

    DefCamera.Update();
    DefGameObjMgr.Update();

    (this->*m_pStateFunc)();

    switch (m_NextSceneType)
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
    for (UINT i = 0; i < m_LoadParams.PlayerArray.size(); ++i)
    {
        auto& it = m_LoadParams.PlayerArray.at(i);

        it.chr_type = CharacterType::__ErrorType;
        it.number = (int)i;
        it.player_type = PlayerType::_Computer;
        it.strong_type = StrongType::_Usual;
    }
    

    for (UINT i = 0; i < m_PlayerNum; ++i)
    {
        m_LoadParams.PlayerArray[i].player_type = PlayerType::_Player;
    }

}

void SceneCharacterSelect::CreateLight()
{

    //���s��
    DirLight* pDir = new DirLight();

    pDir->param.color = Vector3(0.28f, 0.24f, 0.24f);
    pDir->param.vec = Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f)); //Vector3Normalize(Vector3(0.1f, -0.5f, 0.9f));
    pDir->param.Shadow.visible = true;
    pDir->param.Shadow.Near = 5;
    pDir->param.Shadow.Far = 30;
    pDir->param.Shadow.origin = pDir->param.vec*-10.0f + Vector3(0, 0, 10);
    pDir->param.Shadow.Size = 17;

    new StaticGameObjectTemplate<DirLight>(pDir);

    //����
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

    class Renderer :public ForwardRenderer
    {
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

        //�`��(�����I�ɌĂ΂��)
        void Render()
        {
            pTexture->Render(0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight, 0, 0, 1024, 512, RS_COPY, 0xFFFFFFFF, Z);
        }

    };

    new StaticGameObjectTemplate<Renderer>(new Renderer(&m_BackTex));

}

void SceneCharacterSelect::SetCharacterPoint()
{
    //�L�����̃J�[�\���|�C���g����
    {
        SelectPointBase* p;
        Vector2 Center((float)iexSystem::ScreenWidth*0.5f, (float)iexSystem::ScreenHeight*0.5f);
        const float Space = 240.0f;

        Vector2 PosA = Vector2(-300, 200);
        Vector2 PosB = Vector2(-100, 150);

        struct
        {
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

        for (int i = 0; i < (int)ARRAYSIZE(ChrPointData); ++i)
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


void SceneCharacterSelect::SetOtherComputerCharacter(CharacterType::Value force)
{
	if (force != CharacterType::__ErrorType)
	{
		for (auto&& it : m_LoadParams.PlayerArray)
		{
			if (it.player_type != PlayerType::_Computer)
			{
				continue;
			}
			it.chr_type = force;
		}
		return;
	}

    struct
    {
        CharacterType::Value Type;
        bool                 Use;
    }
    CharacterData[4]=
    {
        { CharacterType::_Tennis, false },
        { CharacterType::_Soccer, false },
        { CharacterType::_Baseball, false },
        { CharacterType::_Americanfootball, false },
    };

	// �g���Ă���L�����N�^�����
    for (auto& it : m_LoadParams.PlayerArray)
    {
        if (it.chr_type != CharacterType::__ErrorType)
        {
            for (int i = 0; i < (int)ARRAYSIZE(CharacterData); ++i)
            {
                if (CharacterData[i].Type == it.chr_type)
                {
                    CharacterData[i].Use = true;
                    break;
                }
            }
        }
    }

    for (auto& it : m_LoadParams.PlayerArray)
    {
        if (it.chr_type == CharacterType::__ErrorType)
        {
            for (int i = 0; ARRAYSIZE(CharacterData); ++i)
            {
                if (CharacterData[i].Use == false)
                {
                    CharacterData[i].Use = true;
                    it.chr_type = CharacterData[i].Type;
                    break;
                }
            }
        }
    }

}


//-------------------------------------------------------------------//

void SceneCharacterSelect::State_PreSelect()
{
    if (++m_Timer > 60)
    {
        //�J�[�\�����Z�b�g
        for (UINT i = 0; i < 4; ++i)
        {
            SelectPointBase* pInitPoint = m_ChrPoint[i];

            if (m_LoadParams.PlayerArray.at(i).player_type == PlayerType::_Computer)
            {
                pInitPoint = m_pComputerDefaultPoint;
            }

            SelectCursor* p = new SelectCursor(m_pManager, (controller::CONTROLLER_NUM)i, &m_Texture, RectI(0, 128, 256, 256), pInitPoint);

            {
                const CharacterType::Value Type = p->m_PlayerInfo.chr_type;

                p->m_PlayerInfo = m_LoadParams.PlayerArray.at(i);
                p->m_PlayerInfo.chr_type = Type;
            }

            //����s�\��
            if (p->m_PlayerInfo.player_type == PlayerType::_Computer)
            {
                p->m_PlayerInfo.chr_type = CharacterType::__ErrorType;
                p->m_ControllerNum = controller::__ERROR_CONTROLLER_NUM;
            }

            //�L�����N�^�\���N���X�����
            new SelectCharacterWindow(p);
        }

        //�X�e�[�g�ڍs
        m_pStateFunc = &SceneCharacterSelect::State_Selecting;

        m_Timer = 0;
    }
}

void SceneCharacterSelect::State_Selecting()
{
    //�S���I�����Ă�����
    if (m_pManager->isAllPlayerSelected())
    {
        //�X�e�[�g�ڍs
        m_pStateFunc = &SceneCharacterSelect::State_FadeOut;

        //�����擾
        m_pManager->GetData(m_LoadParams);

        //�����Ă���L�����N�^�f�[�^�ɃR���s���[�^������
		SetOtherComputerCharacter();
    }
    else {
        if ( controller::GetButtonState(controller::button::batu, 0)) {
            //�X�e�[�g�ڍs
            m_pStateFunc = &SceneCharacterSelect::State_BackToOption;
        }
    }
}

void SceneCharacterSelect::State_FadeOut()
{
    if (++m_Timer > 60)
    {
        //�X�e�[�g�ڍs
        m_pStateFunc = &SceneCharacterSelect::State_Change;

        m_NextSceneType = NextSceneType::Next;

        m_Timer = 0;
    }
}

void SceneCharacterSelect::State_BackToOption() {

    if ( ++m_Timer > 60 ) {
        //�X�e�[�g�ڍs
        m_pStateFunc = &SceneCharacterSelect::State_Change;

        m_NextSceneType = NextSceneType::Back;

        m_Timer = 0;
    }

}

void SceneCharacterSelect::State_Change()
{
    
}