#include "GameInitilizer_UsualMatch.h"

#include "../GameSystem/GameSystem.h"

#include "../character/CharacterBase.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterFunction.h"

#include "../character/Tennis/TennisPlayerState_UsualMove.h"
#include "../character/Baseball/BaseballPlayerState.h"
#include "../character/Soccer/SoccerPlayerState.h"
#include "../character/Amefoot/AmefootPlayerState.h"

#include "../GameSystem/GameEventer.h"
#include "../Item/SkillGaugeItemEmitter.h"

#include "../Stage/Ground_Manager.h"
#include "../Stage/Kasenziki_Manager.h"
#include "../Item/Item_BellEmitter.h"
#include "../Item/Item_Bell.h"
#include "../Camera/CameraState.h"


static void CreateCharacter(
    PlayerNum::Value      n,
    PlayerType::Value     pl,
    CharacterType::Value  chr,
    StrongType::Value     str
    )
{
    CharacterBase* pChr = nullptr;

    CharacterBase::PlayerInfo info;

    info.chr_type = chr;
    info.number = n;
    info.player_type = pl;
    info.strong_type = (pl == PlayerType::_Player) ? (StrongType::__ErrorType) : (str);

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
        pChr = new AmefootPlayer(info);
        ((AmefootPlayer*)pChr)->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove((AmefootPlayer*)pChr));
        break;

    }

    pChr->m_Params.pos = DefCharacterMgr.GetRoundStartPos(n);
    chr_func::AngleControll(pChr, Vector3Zero);

}


GameInitializer_UsualMatch::GameInitializer_UsualMatch(StageType Stage, sceneGamePlay::InitParams& param) :
m_StageType(Stage)
{
    m_Param = param;
    m_Param.pInitializer = nullptr;
}

GameInitializer_UsualMatch::~GameInitializer_UsualMatch()
{

}

void GameInitializer_UsualMatch::GameCreate()
{

    {
        GameEventer::Param param;

        param.round = m_Param.Round;
        param.time = m_Param.Time;
        // param.time = 60 * 3;

#if TRUE
        new GameEventer(param, new MatchState::RoundResetCountdown());
#else
        new GameEventer(param, new MatchState::PlayerCharacterDrawCursor());
#endif
        
    }


    //アイテムエミッターの生成
    if(0)
    {
        new SkillGaugeItemEmitter(60 * 60 * 1);

        new Item_BellEmitter(
            60 * 60 * 2  //２分
            );
    }


    //試合中のカメラをセット
    {
        DefCamera.SetNewState(new CameraStateGamePlay());
    }


    //ステージ作成
    switch (m_StageType)
    {
    case StageType::Kasennziki:
        (new Kasennziki_Manager(m_Param.Round))->CreateStage();
        break;

    case StageType::School:
        (new Ground_Manager(m_Param.Round))->CreateStage();
        break;

    default:

        MyAssert(false, "認識できないステージタイプ");
        break;
    }


    //キャラクタ作成
    for (auto& it : m_Param.PlayerArray)
    {
        if (it.number < 0)
        {
            continue;
        }

        CreateCharacter((PlayerNum::Value)it.number, it.player_type, it.chr_type,it.strong_type);
    }
}
