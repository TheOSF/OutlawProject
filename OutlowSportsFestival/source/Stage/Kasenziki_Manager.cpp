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
#include "SkyObject.h"
#include "../GameSystem/ResourceManager.h"

const char* Kasennziki_Manager::StageObjFileName = "DATA\\Stages\\Stage1\\StageObjData.txt";

Kasennziki_Manager::Kasennziki_Manager(UINT night_round) :
m_SunsetRound(night_round),
m_RoundCount(1),
m_DeleyFrame(0),
m_pSkyObject(nullptr)
{
    //���\�[�X�ɂ܂Ƃ߂ēo�^
    StageResourceLoadFaction::LoadStage1Object();

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
        MeshCollider* C = new MeshCollider(pStageMesh, nullptr, false, CollisionManager::RayType::_Ball | CollisionManager::RayType::_Usual);

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

        {
            //�L�����N�^��p�̓����蔻��X�e�[�W
            MeshCollider* C2 = new MeshCollider(new iexMesh("DATA\\Stages\\Stage1\\kasenziki\\kasenziki_ChrHit.IMO"), nullptr, true, CollisionManager::RayType::_Character);
            C2->SetWorldMatrix(m);
            new StaticGameObjectTemplate<MeshCollider>(C2);
        }



        //�����蔻��̂���X�e�[�W�ɂ���
        new HitStageObject(
            R,
            C
            );

        {
            iexMesh* pHit = DefResource.Get(Resource::MeshType::Stage1_HitPhysic);

            pHit->SetScale(scale, scale, scale);
            pHit->SetAngle(0, PI, PI);

            //�y��̃X�e�[�W�𕨗��G���W���ɓo�^
            DefBulletSystem.AddRigidMesh(
                pHit,
                0,
                RigidBody::ct_static,
                0.5f,
                0.5f,
                Vector3(0, 0, 0),
                Vector3(0, 0, 0)
                );
        }
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
        //���C�e�B���O�Z�b�g
        /*
        if (m_SunsetRound == 1)
        {
            SetSunsetLight();
            SetSunsetObject();
        }
        else
        {
            SetNoonLight();
            SetNoonObject();
        }
        */

        SetSunsetLight();
        SetSunsetObject();
    }


    

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
    if (m_RoundCount == m_SunsetRound)
    {
        //���C�e�B���O��[����
        SetSunsetLight();
    }

    //�I�u�W�F�N�g�̍Ĕz�u
    if (m_RoundCount >= m_SunsetRound)
    {
        SetSunsetObject();
    }
    else
    {
        SetNoonObject();
    }
}

void Kasennziki_Manager::SetSunsetObject()
{
    //���ǉ�
    if (m_pSkyObject == nullptr)
    {
        m_pSkyObject = new SkyObject("DATA\\Stages\\Stage1\\Sky\\Sky.imo");
    }

    //�ǂݍ���
    StageEditer::Load(StageObjFileName);
}

void Kasennziki_Manager::SetNoonObject()
{
    ////����폜
    //if (m_pSkyObject != nullptr)
    //{
    //    m_pSkyObject->m_DeleteFlg = true;
    //    m_pSkyObject = nullptr;
    //}

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
    m_DirLight.param.Shadow.Far = 250;
    m_DirLight.param.Shadow.origin = m_DirLight.param.vec*-150.0f;
    m_DirLight.param.Shadow.Size = 120;

    m_AmbientLight.param.color = Vector3(0.37f, 0.26f, 0.26f);
    m_AmbientLight.param.Occlusion.SamplingSize = 0.1f;
    m_AmbientLight.param.Occlusion.Enable = false;

    DefMatchLightManager.UpdateLightColor(&m_AmbientLight.param.color, m_AmbientLight.param.color);
    DefMatchLightManager.UpdateLightColor(&m_DirLight.param.color, m_DirLight.param.color);
}

void Kasennziki_Manager::SetNoonLight()
{
    //�����̃��C�e�B���O���Z�b�g
    m_DirLight.param.color = Vector3(0.33f, 0.22f, 0.22f);
    m_DirLight.param.vec = Vector3Normalize(Vector3(0.8f, -2, 0.2f));
    m_DirLight.param.Shadow.visible = true;
    m_DirLight.param.Shadow.Near = 5;
    m_DirLight.param.Shadow.Far = 150;
    m_DirLight.param.Shadow.origin = m_DirLight.param.vec*-50.0f;
    m_DirLight.param.Shadow.Size = 120;

    m_AmbientLight.param.color = Vector3(0.29f, 0.29f, 0.29f);
    m_AmbientLight.param.Occlusion.SamplingSize = 0.1f;
    m_AmbientLight.param.Occlusion.Enable = false;

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