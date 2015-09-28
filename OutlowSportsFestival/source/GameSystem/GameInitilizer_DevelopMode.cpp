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


    //キャラクタ作成
    {

        CreateCharacter((PlayerNum::Value)0, PlayerType::_Player, CharacterType::_Tennis);
        CreateCharacter((PlayerNum::Value)1, PlayerType::_Computer, CharacterType::_Tennis);

        CreateCharacter((PlayerNum::Value)2, PlayerType::_Computer, CharacterType::_Tennis);
        CreateCharacter((PlayerNum::Value)3, PlayerType::_Computer, CharacterType::_Tennis);
    }


    {
        GameEventer::Param param;

        param.round = 3;
        param.time = 60 * 60 * 3; //３分

        new GameEventer(param, new MatchState::RoundResetCountdown());
    }


    {
        //ステージ作成
        pStageMesh = new iexMesh("DATA\\STAGE\\Stage.IMO");

        //当たり判定のあるステージにする
        new HitStageObject(
            new MeshRenderer(pStageMesh, true, MeshRenderer::RenderType::UseColor),
            new MeshCollider(pStageMesh, new MeshCollider::HitEvent)
            );


        //ステージの煙の演出
        new StageSmokeEmitter(
            Vector3(-50,0,-50),
            Vector3(50, 5, 50),
            0x80FFA080,
            120,
            50
            );

    }

    {// Bullet

        DefBulletSystem.StartUp();
        DefBulletSystem.InitializeBulletPhysics(btVector3(0, -9.8f, 0), iexSystem::Device);

        //土台のステージ
        DefBulletSystem.AddRigidMesh(
            pStageMesh,
            10000.0f,
            RigidBody::ct_kinematic,
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

    {
        //ＵＩ用文字列画像
        DefResource.Regist(
            Resource::TextureType::UI_strings,
            new iex2DObj("DATA\\Texture\\title_raund_nowlording.png")
            );
    }

    {
        //デバッグ描画用の球メッシュ
        DefResource.Regist(
            Resource::MeshType::Sphere,
            new iexMesh("DATA\\Mesh\\sphere.imo")
            );
    }

    {
        //ボール登録(最終的に登場するキャラクタのボールのみ読み込むようにしたほうが良い)
        
        DefResource.Regist(
            Resource::MeshType::Amefoot_ball,
            new iexMesh("DATA\\CHR\\Soccer_ball\\soccer_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::BaseBall_ball,
            new iexMesh("DATA\\CHR\\golf_ball\\golf_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Lacrosse_ball,
            new iexMesh("DATA\\CHR\\Tennis_ball\\Tennis_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Soccer_ball,
            new iexMesh("DATA\\CHR\\Soccer_ball\\soccer_ball.imo")
            );


        DefResource.Regist(
            Resource::MeshType::Tennis_ball,
            new iexMesh("DATA\\CHR\\Tennis_ball\\Tennis_ball.imo")
            );

        DefResource.Regist(
            Resource::MeshType::Volley_ball,
            new iexMesh("DATA\\CHR\\Soccer_ball\\soccer_ball.imo")
            );
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

            D->param.color = Vector3(0.2f, 0.25f, 0.25f);
            D->param.vec = Vector3Normalize(Vector3(0.2f, -2, 0.5f));
            D->param.Shadow.visible = true;
            D->param.Shadow.Near = 5;
            D->param.Shadow.Far = 150;
            D->param.Shadow.origin = DefCamera.m_Position + Vector3(0, 20, 20);
            D->param.Shadow.Size = 120;

            new DebugControllGameObject(&D->param.color, 0, 0.01f, "DirColor", 'D');
            new StaticGameObjectTemplate<DirLight>(D);
        }

        {
            AmbientLight* A = new AmbientLight;

            A->param.color = Vector3(0.38f, 0.24f, 0.24f);
            A->param.Occlusion.SamplingSize = 0.025f;
            A->param.Occlusion.Enable = true;

            new DebugControllGameObject(nullptr, &A->param.Occlusion.SamplingSize, 0.01f, "AmbSize", 'S');
            new DebugControllGameObject(&A->param.color, 0, 0.01f, "AmbColor", 'A');
            new StaticGameObjectTemplate<AmbientLight>(A);
        }
    }
   
}
