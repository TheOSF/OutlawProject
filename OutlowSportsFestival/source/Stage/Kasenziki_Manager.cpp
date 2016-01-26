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
    //リソースにまとめて登録
    StageResourceLoadFaction::LoadStage1Object();

    //ライト登録
    DefMatchLightManager.AddManageLightValue(&m_DirLight.param.color);
    DefMatchLightManager.AddManageLightValue(&m_AmbientLight.param.color);

    //デバッグ用ライティング調整クラス
    new DebugControllGameObject(&m_DirLight.param.color, 0, 0.01f, "DirColor", 'D');
    new DebugControllGameObject(&m_AmbientLight.param.color, 0, 0.01f, "AmbColor", 'A');


    //とりあえず夕焼けに
    SetSunsetLight();

}


Kasennziki_Manager::~Kasennziki_Manager()
{
    //リソースから削除
    StageResourceLoadFaction::ReleaseStage1Object();
}


//-----------------Private----------------------//

void Kasennziki_Manager::CreateStage()
{
    //土台なるステージをセット
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
            //キャラクタ専用の当たり判定ステージ
            MeshCollider* C2 = new MeshCollider(new iexMesh("DATA\\Stages\\Stage1\\kasenziki\\kasenziki_ChrHit.IMO"), nullptr, true, CollisionManager::RayType::_Character);
            C2->SetWorldMatrix(m);
            new StaticGameObjectTemplate<MeshCollider>(C2);
        }



        //当たり判定のあるステージにする
        new HitStageObject(
            R,
            C
            );

        {
            iexMesh* pHit = DefResource.Get(Resource::MeshType::Stage1_HitPhysic);

            pHit->SetScale(scale, scale, scale);
            pHit->SetAngle(0, PI, PI);

            //土台のステージを物理エンジンに登録
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

   

    //夕焼け時のオブジェクトをセット
    if (false)
    {
        //ステージエディット
        StageEditer* p = new StageEditer();

        p->LoadAndEdit(StageObjFileName);
    }
    else
    {
        //ライティングセット
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


    

    //車メーカーをセット
    new StageCarEmitter(0);

    //川を追加

    new RiverObject(
        Vector3(0, -1, -40),
        Vector3(8, 1, 8),
        Vector3(0, 0, 0),
        Vector2(0.001f, 0.0f)
        );

}



void Kasennziki_Manager::RoundReset()
{
    //ラウンドカウント更新
    ++m_RoundCount;

    //ライティングチェック
    if (m_RoundCount == m_SunsetRound)
    {
        //ライティングを夕方に
        SetSunsetLight();
    }

    //オブジェクトの再配置
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
    //空を追加
    if (m_pSkyObject == nullptr)
    {
        m_pSkyObject = new SkyObject("DATA\\Stages\\Stage1\\Sky\\Sky.imo");
    }

    //読み込み
    StageEditer::Load(StageObjFileName);
}

void Kasennziki_Manager::SetNoonObject()
{
    ////空を削除
    //if (m_pSkyObject != nullptr)
    //{
    //    m_pSkyObject->m_DeleteFlg = true;
    //    m_pSkyObject = nullptr;
    //}

    //読み込み
    StageEditer::Load(StageObjFileName);
}

void Kasennziki_Manager::SetSunsetLight()
{
    //夕焼け時のライティングをセット
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
    //昼時のライティングをセット
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

    //リセットメッセージが来てから１フレームだけリセットするフレームを遅らせる
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