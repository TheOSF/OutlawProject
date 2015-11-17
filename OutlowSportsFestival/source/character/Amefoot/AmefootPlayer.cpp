
#include "AmefootPlayer.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ アメフト ] キャラクタークラス
//-----------------------------------------------------------------------------------------
AmefootPlayer::AmefootPlayer(const PlayerInfo& info) :
     CharacterBase(info) ,
     m_Renderer(new BlendAnimationMesh("DATA\\CHR\\Soccer\\Player_S.iem")) ,
     m_ModelSize(0.052f) ,
     m_pStateMachine(new AmefootStateMachine(this))
{

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
     return true;
}
//-----------------------------------------------------------------------------------------
void AmefootPlayer::SetState(AmefootState* state , bool important)
{
     m_pStateMachine->set_state(state , important);
}
//-----------------------------------------------------------------------------------------

