
#include "AmefootPlayer.h"
#include "AmefootPlayerState.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ アメフト ] キャラクタークラス
//-----------------------------------------------------------------------------------------
AmefootPlayer::AmefootPlayer(const PlayerInfo& info) :
CharacterBase(info, new BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Americanfootball))),
     m_pStateMachine(new AmefootStateMachine(this)),
     m_DontThrowBallTimer(0)
{
    m_ModelSize = (0.058f);
}
//-----------------------------------------------------------------------------------------
AmefootPlayer::~AmefootPlayer()
{
     delete m_pStateMachine;
}
//-----------------------------------------------------------------------------------------
bool AmefootPlayer::Update()
{
     m_pStateMachine->state_execute();

     CharacterBase::BaseUpdate();

     //ボール撃てないたいまーの更新
     if (m_DontThrowBallTimer > 0)
     {
         --m_DontThrowBallTimer;
     }

     return true;
}
//-----------------------------------------------------------------------------------------
bool AmefootPlayer::CharacterMsg(MsgType msg)
{
     switch ( msg )
     {
     case GameObjectBase::_PlayStart:
          break;
     case GameObjectBase::_RoundReset:
         SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(this));
          break;
     case GameObjectBase::_GameSet:
          break;
     case GameObjectBase::_WinPose:
          break;
     case GameObjectBase::_LosePose:
          break;
     default:
          break;
     }

     return m_pStateMachine->Msg(msg);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayer::SetState(AmefootState* state , int important)
{
     m_pStateMachine->set_state(state , important);
}
//-----------------------------------------------------------------------------------------

void AmefootPlayer::SetDontThrowBallTimer(int Frame)
{
    m_DontThrowBallTimer = Frame;
}

bool AmefootPlayer::isCanThrowBall()const
{
    return m_DontThrowBallTimer <= 0;
}