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

#include "../../Input/GamePad/GamePadManager.h"
#include "../Stage/Kasenziki_Manager.h"
#include "../character/Soccer/Computer/SoccerComputerMove.h"


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
    info.strong_type = (pl == PlayerType::_Player) ? (StrongType::__ErrorType) : (StrongType::_Strong);

    switch (chr)
    {
    case CharacterType::_Tennis:
        pChr = new TennisPlayer(info);
        ((TennisPlayer*)pChr)->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove((TennisPlayer*)pChr));
        break;

    case CharacterType::_Baseball:
        pChr = new BaseballPlayer(info);
        ((BaseballPlayer*)pChr)->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove((BaseballPlayer*)pChr));
        break;

    case CharacterType::_Soccer:
        pChr = new SoccerPlayer(info);
        ((SoccerPlayer*)pChr)->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove((SoccerPlayer*)pChr));
        break;

    case CharacterType::_Americanfootball:
        //未作成

        break;

    //case CharacterType::_Lacrosse:
    //    pChr = new LacrossePlayer(info);
    //    ((LacrossePlayer*)pChr)->SetState(new LacrosseState_PlayerControllMove());
    //    break;

    //case CharacterType::_Volleyball:
    //    pChr = new VolleyBallPlayer(info);
    //    ((VolleyBallPlayer*)pChr)->SetState(new VolleyBallState_PlayerControll_Move());
    //    break;
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
        //エフェクト読み込み
        EffectResource::Load();
    }


    {
        GameEventer::Param param;

        param.round = 3;
        param.time = 60 * 60 * 1; // 1 分
        // param.time = 60 * 3;

        new GameEventer(param, new MatchState::RoundResetCountdown());
    }


    {
        Kasennziki_Manager* p = new Kasennziki_Manager(3);
        p->CreateStage();
    }


    //キャラクタ作成
    {
        CreateCharacter((PlayerNum::Value)0, PlayerType::_Player, CharacterType::_Tennis);
        CreateCharacter((PlayerNum::Value)1, PlayerType::_Computer, CharacterType::_Baseball);
    
        CreateCharacter((PlayerNum::Value)2, PlayerType::_Computer, CharacterType::_Soccer);
        CreateCharacter((PlayerNum::Value)3, PlayerType::_Computer, CharacterType::_Soccer);
    }
}
