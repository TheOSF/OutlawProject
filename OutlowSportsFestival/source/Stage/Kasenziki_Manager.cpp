#include "Kasenziki_Manager.h"
#include "StageResourceLoadFaction.h"
#include "../Render/MeshRenderer.h"
#include "../Collision/MeshCollider.h"
#include "HitStageObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "StageCarEmitter.h"
#include "../utillity/DebugControllGameObject.h"
#include "../utillity/StaticGameObjectTemplate.h"
#include "../GameSystem/MatchLightManager.h"
#include "StageEdit.h"
#include "RiverObject.h"


const char* Kasennziki_Manager::StageObjFileName = "DATA\\Stages\\Stage1\\StageObjData.txt";

Kasennziki_Manager::Kasennziki_Manager(UINT night_round) :
m_NightRound(night_round),
m_RoundCount(1),
m_DeleyFrame(0)
{
    //���\�[�X�ɂ܂Ƃ߂ēo�^
    StageResourceLoadFaction::LoadStage1Object();

    //���C�g������
    for (int i = 0; i < _NumSpotLight; ++i)
    {
        m_pSpotLight[i] = new SpotLight();
        m_pSpotLight[i]->Visible = false;
    }

    //���C�g�o�^
    DefMatchLightManager.AddManageLightValue(&m_DirLight.param.color);
    DefMatchLightManager.AddManageLightValue(&m_AmbientLight.param.color);

    //�f�o�b�O�p���C�e�B���O�����N���X
    new DebugControllGameObject(&m_DirLight.param.color, 0, 0.01f, "DirColor", 'D');
    new DebugControllGameObject(&m_AmbientLight.param.color, 0, 0.01f, "AmbColor", 'A');


    //�Ƃ肠�����[�Ă���
    SetSunsetLight();

}


Kasennziki_Manager::~Kasennziki_Manager()
{
    for (int i = 0; i < _NumSpotLight; ++i)
    {
        delete m_pSpotLight[i];
    }
    //���\�[�X����폜
    StageResourceLoadFaction::ReleaseStage1Object();
}


//-----------------Private----------------------//

void Kasennziki_Manager::CreateStage()
{
    //�y��Ȃ�X�e�[�W���Z�b�g
    {
        iexMesh* pStageMesh;
        pStageMesh = new iexMesh("DATA\\Stages\\Stage1\\kasenziki\\kasenziki.IMO");

        Matrix m;

        const float scale = 0.1f;

        MeshRenderer* R = new MeshRenderer(pStageMesh, true, MeshRenderer::RenderType::UseColorSpecular, MeshRenderer::GbufRenderType::UseNormal);
        MeshCollider* C = new MeshCollider(pStageMesh, new MeshCollider::HitEvent);

        pStageMesh->SetScale(scale, scale, scale);
        pStageMesh->SetAngle(0, PI, PI);

        D3DXMatrixScaling(&m, scale, scale, scale);
        {
            Matrix s;
            D3DXMatrixRotationY(&s, PI);
            m *= s;
        }

        R->SetMatrix(m);
        C->SetWorldMatrix(m);

        //�����蔻��̂���X�e�[�W�ɂ���
        new HitStageObject(
            R,
            C
            );

        //�y��̃X�e�[�W�𕨗��G���W���ɓo�^
        DefBulletSystem.AddRigidMesh(
            pStageMesh,
            0,
            RigidBody::ct_static,
            0.5f,
            0.5f,
            Vector3(0, 0, 0),
            Vector3(0, 0, 0)
            );
    }
    

    //�[�Ă����̃I�u�W�F�N�g���Z�b�g
    if (false)
    {
        //�X�e�[�W�G�f�B�b�g
        StageEditer* p = new StageEditer();

        p->LoadAndEdit(StageObjFileName);
    }
    else
    {
        //�ʏ�̃Z�b�g
        SetSunsetObject();
    }


    //�[�Ă����̃��C�e�B���O�Z�b�g
    SetSunsetLight();
    //SetNightLight();

    //�ԃ��[�J�[���Z�b�g
    new StageCarEmitter(0);

    //���ǉ�
    new RiverObject(
        Vector3(0, -1, -40),
        Vector3(8, 1, 8),
        Vector3(0, 0, 0),
        Vector2(0.001f, 0.0f)
        );
}



void Kasennziki_Manager::RoundReset()
{
    //���E���h�J�E���g�X�V
    ++m_RoundCount;

    //���C�e�B���O�`�F�b�N
    if (m_RoundCount == m_NightRound)
    {
        //���C�e�B���O����
        SetNightLight();
    }

    //�I�u�W�F�N�g�̍Ĕz�u
    if (m_RoundCount >= m_NightRound)
    {
        SetNightObject();
    }
    else
    {
        SetSunsetObject();
    }
}

void Kasennziki_Manager::SetSunsetObject()
{
    //�ǂݍ���
    StageEditer::Load(StageObjFileName);
}

void Kasennziki_Manager::SetNightObject()
{
    //�ǂݍ���
    StageEditer::Load(StageObjFileName);
}

void Kasennziki_Manager::SetSunsetLight()
{
    //�[�Ă����̃��C�e�B���O���Z�b�g
    m_DirLight.param.color = Vector3(0.3f, 0.2f, 0.2f);
    m_DirLight.param.vec = Vector3Normalize(Vector3(0.8f, -1, 0.2f));
    m_DirLight.param.Shadow.visible = true;
    m_DirLight.param.Shadow.Near = 5;
    m_DirLight.param.Shadow.Far = 150;
    m_DirLight.param.Shadow.origin = m_DirLight.param.vec*-50.0f;
    m_DirLight.param.Shadow.Size = 120;

    m_AmbientLight.param.color = Vector3(0.37f, 0.26f, 0.26f);
    m_AmbientLight.param.Occlusion.SamplingSize = 0.1f;
    m_AmbientLight.param.Occlusion.Enable = false;

    for (int i = 0; i < _NumSpotLight; ++i)
    {
        m_pSpotLight[i]->Visible = false;
    }

    DefMatchLightManager.UpdateLightColor(&m_AmbientLight.param.color, m_AmbientLight.param.color);
    DefMatchLightManager.UpdateLightColor(&m_DirLight.param.color, m_DirLight.param.color);
}

void Kasennziki_Manager::SetNightLight()
{
    //�鎞�̃��C�e�B���O���Z�b�g

    m_DirLight.param.color = Vector3(0.08f, 0.08f, 0.08f);
    m_DirLight.param.vec = Vector3Normalize(Vector3(0.8f, -1, 0.2f));
    m_DirLight.param.Shadow.visible = false;
    m_DirLight.param.Shadow.Near = 5;
    m_DirLight.param.Shadow.Far = 150;
    m_DirLight.param.Shadow.origin = m_DirLight.param.vec*-50.0f;
    m_DirLight.param.Shadow.Size = 120;


    m_AmbientLight.param.color = Vector3(0.05f, 0.05f, 0.05f);
    m_AmbientLight.param.Occlusion.SamplingSize = 0.1f;
    m_AmbientLight.param.Occlusion.Enable = false;

    {
        m_pSpotLight[0]->Visible = true;

        m_pSpotLight[0]->param.origin = Vector3(0, 10, 45);
        m_pSpotLight[0]->param.target = Vector3(0, -2, -20);
        m_pSpotLight[0]->param.color = Vector3(1.0f, 1.0f, 1.0f)*2.0f;
        m_pSpotLight[0]->param.size = 20;
        m_pSpotLight[0]->param.Shadow.visible = true;
    }

    {
        m_pSpotLight[1]->Visible = true;
                     
        m_pSpotLight[1]->param.origin = Vector3(0, 10, -45);
        m_pSpotLight[1]->param.target = Vector3(0, -2, 20);
        m_pSpotLight[1]->param.color = Vector3(1.0f, 1.0f, 1.0f)*2.0f;
        m_pSpotLight[1]->param.size = 20;
        m_pSpotLight[1]->param.Shadow.visible = true;
    }

    DefMatchLightManager.UpdateLightColor(&m_AmbientLight.param.color, m_AmbientLight.param.color);
    DefMatchLightManager.UpdateLightColor(&m_DirLight.param.color, m_DirLight.param.color);
}


bool Kasennziki_Manager::Update()
{
    //���Z�b�g���b�Z�[�W�����Ă���P�t���[���������Z�b�g����t���[����x�点��
    if (m_DeleyFrame > 0)
    {
        --m_DeleyFrame;

        if (m_DeleyFrame == 1)
        {
            RoundReset();
        }
    }
    return true;
}

bool Kasennziki_Manager::Msg(MsgType msg)
{
    switch (msg)
    {
    case _RoundReset:
        m_DeleyFrame = 3; 
        return true;
    }

    return false;
}