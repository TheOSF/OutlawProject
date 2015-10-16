#include "GameInitilizer_DevelopMode.h"

#include	"iextreme.h"
#include	"../IexSystem/System.h"
#include	"debug\DebugFunction.h"
#include	"../Ball/Ball.h"
#include	"../Camera/Camera.h"
#include	"../character/CharacterManager.h"
#include    "../Collision/Collision.h"
#include    "../Collision/GameObjectCollisionMesh.h"
#include    "../GameSystem/ResourceManager.h"
#include    "../debug/DebugDraw.h"
#include    "../Library/Bullet/BulletSystem.h"

#include	"../character/Tennis/TennisPlayer.h"
#include	"../character/Tennis/TennisPlayerState_UsualMove.h"

#include	"../character/Soccer/SoccerPlayer.h"
#include	"../character/Soccer/SoccerPlayerState.h"

#include	"../character/Baseball/BaseballPlayer.h"
#include	"../character/Baseball/BaseballPlayerState.h"

#include    "../character/Amefoot/AmefootPlayer.h"
#include    "../character/Amefoot/AmefootPlayerState.h"

#include    "../character/Lacrosse/LacrossePlayer.h"
#include    "../character/Lacrosse/LacrossePlayerState.h"

#include    "../character/VolleyBall/VolleyBallPlayer.h"
#include    "../character/VolleyBall/VolleyBallPlayerState.h"

#include    "../Render/LightObject.h"

#include    "../Effect/EffectResourceLoad.h"

// Effekseer
#include "../Library/Effekseer/EffekseerSystem.h"
#include "../Library/Effekseer/EffekseerEffectManager.h"
#include "../Library/Effekseer/EffekseerEffect.h"

// Bullet
#include "../Library/Bullet/BulletSystem.h"
#include "../Library/Bullet/BulletUpdateGameobject.h"

#include "../GameSystem/GameEventer.h"
#include "../character/CharacterFunction.h"
#include "../Render/MeshRenderer.h"

#include "../GameSystem/GameInitializer.h"
#include "../GameSystem/GameInitilizer_DevelopMode.h"

#include "../utillity/StaticGameObjectTemplate.h"
#include "../utillity/DebugControllGameObject.h"

#include "../Stage/HitStageObject.h"

#include "../Effect/ParticleRenderer.h"
#include "../Render/BlurObject.h"
#include "../Effect/StageSmokeEmitter.h"

#include "../Effect/ParticleManagerUpdater.h"
#include "LightEventExecuter.h"
\
#include "../Stage/StageResourceLoadFaction.h"
#include "../Stage/StageObjectFactory.h"

static void CreateCharacter(
    PlayerNum::Value      n,
    PlayerType::Value     pl,
    CharacterType::Value  chr)
{
    CharacterBase* pChr = nullptr;

    CharacterBase::PlayerInfo info;

    info.chr_type = chr;
    info.number = n;
    info.player_type = pl;
    info.strong_type = (pl == PlayerType::_Player) ? (StrongType::__ErrorType) : (StrongType::__ErrorType);

    switch (chr)
    {
    case CharacterType::_Tennis:
        pChr = new TennisPlayer(info);
        ((TennisPlayer*)pChr)->SetState(new TennisState_PlayerControll_Move());
        break;

    case CharacterType::_Baseball:
        pChr = new BaseballPlayer(info);
        ((BaseballPlayer*)pChr)->SetState(new BaseballState_PlayerControll_Move());
        break;

    case CharacterType::_Soccer:
        pChr = new SoccerPlayer(info);
        ((SoccerPlayer*)pChr)->SetState(new SoccerState_PlayerControll_Move());
        break;

    case CharacterType::_Americanfootball:
        //未作成

        break;

    case CharacterType::_Lacrosse:
        pChr = new LacrossePlayer(info);
        ((LacrossePlayer*)pChr)->SetState(new LacrosseState_PlayerControllMove());
        break;

    case CharacterType::_Volleyball:
        pChr = new VolleyBallPlayer(info);
        ((VolleyBallPlayer*)pChr)->SetState(new VolleyBallState_PlayerControll_Move());
        break;
    }

    pChr->m_Params.pos = DefCharacterMgr.GetRoundStartPos(n);
    chr_func::AngleControll(pChr, Vector3Zero);

}


GameInitializer_DevelopMode::GameInitializer_DevelopMode()
{

}

GameInitializer_DevelopMode::~GameInitializer_DevelopMode()
{

}

void GameInitializer_DevelopMode::GameCreate()
{
    iexMesh* pStageMesh;

    //	環境設定
    iexLight::SetAmbient(0x404040);
    iexLight::SetFog(800, 1000, 0);

    Vector3 dir(1.0f, -1.0f, -0.5f);
    dir.Normalize();
    iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

    DefCamera.m_Position = Vector3(0, 40, -55);
    DefCamera.m_Target = Vector3(0, 4, 0);

    //当たり判定のデバッグ描画を有効に
    DefDamageMgr.m_DebugDrawVisible = true;


    {// Bullet

        DefBulletSystem.StartUp();
        DefBulletSystem.InitializeBulletPhysics(btVector3(0, -9.8f, 0)*3.5f, iexSystem::Device);

        //更新クラスの作成
        new BulletUpdateGameobject();
    }


    {
        //ＵＩ用文字列画像
        DefResource.Regist(
            Resource::TextureType::UI_strings,
            new iex2DObj("DATA\\Texture\\title_raund_nowlording.png")
            );

        //ＵＩ用プレイヤーネーム
        DefResource.Regist(
            Resource::TextureType::UI_player_name,
            new iex2DObj("DATA\\Texture\\PlayerNo.png")
            );
    }

    {
        //デバッグ描画用の球メッシュ
        DefResource.Regist(
            Resource::MeshType::Sphere,
            new iexMesh("DATA\\Mesh\\sphere.imo")
            );

        //デバッグ描画用のカプセルメッシュ
        DefResource.Regist(
            Resource::MeshType::Pole,
            new iexMesh("DATA\\Mesh\\Capsure.imo")
            );
    }


    {
        //ボール登録(最終的に登場するキャラクタのボールのみ読み込むようにしたほうが良い)

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
            Resource::MeshType::Volley_ball,
            new iexMesh("DATA\\CHR\\Ball\\soccer_ball.imo")
            );
    }

    {
        //ステージ１のメッシュをリソースに登録
        StageResourceLoadFaction::LoadStage1Object();
    }

    //描画デバイス呼び出し
    {
        //ステージ作成
        if (false)
        {
            pStageMesh = new iexMesh("DATA\\STAGE\\Stage.IMO");

            //当たり判定のあるステージにする
            new HitStageObject(
                new MeshRenderer(pStageMesh, true, MeshRenderer::RenderType::UseColor),
                new MeshCollider(pStageMesh, new MeshCollider::HitEvent)
                );
        }
        else
        {
            pStageMesh = new iexMesh("DATA\\Stages\\Kasenjiki.IMO");

            Matrix m;

            const float scale = 0.1f;

            MeshRenderer* R = new MeshRenderer(pStageMesh, true, MeshRenderer::RenderType::UseColor);
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

            //当たり判定のあるステージにする
            new HitStageObject(
                R,
                C
                );
        }


        {
            //テスト用にオブジェクトを配置
            
            //StageObjFactory::CreateBench(
            //    Vector3(10, 2, 0),
            //    Vector3(0, 0, 0)
            //    );

            for (int i = 0; i < 2; ++i)
            {

                StageObjFactory::CreateBench(
                    Vector3((frand() - 0.5f) * 60, 2.0f, (frand() - 0.5f) * 60),
                    Vector3(0, PI / 2, 0)
                    );

            }

            for (int i = 0; i < 10; ++i)
            {
                StageObjFactory::CreateCone(
                    Vector3((frand() - 0.5f) * 60, 2.5f, (frand() - 0.5f) * 0),
                    //Vector3(frand()*PI, frand()*PI, frand()*PI)
                    Vector3Zero
                    );
            }

            for (int i = 0; i < 5; ++i)
            {
                StageObjFactory::CreatePipe(
                    Vector3((frand() - 0.5f) * 60, 10.5f, (frand() - 0.5f) * 0),
                    //Vector3(frand()*PI, frand()*PI, frand()*PI)
                    Vector3Zero
                    );
            }
        }


        ////ステージの煙の演出
        //new StageSmokeEmitter(
        //    Vector3(-50,0,-50),
        //    Vector3(50, 5, 50),
        //    0x80FFA080,
        //    120,
        //    50
        //    );


        //土台のステージを物理エンジンに登録
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

    {
        //エフェクト読み込み
        EffectResource::Load();
    }

    {
        //パーティクルアップデーター生成
        new ParticleManagerUpdater();
    }

    if (0)
    {
        //デバッグ用ダメージクラス

        DamageShpere* d = new DamageShpere();

        d->type = DamageBase::Type::_WeekDamage;
        d->m_Param.pos.y = 2.5f;
        d->vec.x = 0.25f;
        d->Value = 25;

        new StaticGameObjectTemplate<DamageShpere>(d);
    }

    Vector3 *pDirLightColor = nullptr;

    {
        //ライティング設定
        {
            HemiLight* H = new HemiLight;

            H->param.GroundColor = Vector3(0, 0, 0);
            H->param.SkyColor = Vector3(0, 0, 0);
            H->param.Up = Vector3(0, 1, 0);

            new StaticGameObjectTemplate<HemiLight>(H);
        }

        {
            DirLight* D = new DirLight;

            D->param.color = Vector3(0.3f, 0.2f, 0.2f);
            D->param.vec = Vector3Normalize(Vector3(0.8f, -1, 0.2f));
            D->param.Shadow.visible = true;
            D->param.Shadow.Near = 5;
            D->param.Shadow.Far = 150;
            D->param.Shadow.origin = D->param.vec*-50.0f;
            D->param.Shadow.Size = 120;

            pDirLightColor = &D->param.color;

            new DebugControllGameObject(&D->param.color, 0, 0.01f, "DirColor", 'D');
            new StaticGameObjectTemplate<DirLight>(D);
        }

        {
            AmbientLight* A = new AmbientLight;

            A->param.color = Vector3(0.37f, 0.26f, 0.26f);
          //  A->param.color = Vector3(0.26f, 0.26f, 0.26f);
            A->param.Occlusion.SamplingSize = 0.025f;
            A->param.Occlusion.Enable = false;

            new DebugControllGameObject(&A->param.color, 0, 0.01f, "AmbColor", 'A');
            new StaticGameObjectTemplate<AmbientLight>(A);
        }
    }
   

    {
        GameEventer::Param param;

        param.round = 3;
        param.time = 60 * 60 * 3; //３分

        new GameEventer(param, new MatchState::RoundResetCountdown(), pDirLightColor);
    }


    //キャラクタ作成
    {
        CreateCharacter((PlayerNum::Value)0, PlayerType::_Player, CharacterType::_Tennis);
        CreateCharacter((PlayerNum::Value)1, PlayerType::_Computer, CharacterType::_Tennis);

        CreateCharacter((PlayerNum::Value)2, PlayerType::_Computer, CharacterType::_Tennis);
        CreateCharacter((PlayerNum::Value)3, PlayerType::_Computer, CharacterType::_Tennis);

    }
}
