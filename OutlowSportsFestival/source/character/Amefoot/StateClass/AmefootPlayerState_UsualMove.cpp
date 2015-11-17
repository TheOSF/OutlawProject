#include "AmefootPlayerState_UsualMove.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"
#include "../../CharacterFunction.h"
#include "../../CharacterHitEventFunc.h"
#include "../AmefootUsualHitEvent.h"

//-----------------------------------------------------------------------------------------
// MoveEvent
//-----------------------------------------------------------------------------------------
typedef class AmefootPlayerState_UsualMove::MoveEvent : public CharacterUsualMove::MoveEvent
{
public:
     MoveEvent(AmefootPlayer* pAmefoot);

     void Update(bool isRun , RATIO speed)override;

     void RunStart()override;

     void StandStart()override;

     void RunEnd()override;

private:
     AmefootPlayer* m_pAmefoot;

}AmefootUsualMove_MoveEvent;
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_UsualMove
// [ アメフト ] 通常移動するステートクラス
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Enter(AmefootPlayer* pCharacter)
{
     CharacterUsualMove::Params params;
     params.Acceleration = 0.13f;
     params.DownSpeed = 0.20f;
     params.MaxSpeed = 0.23f;
     params.TurnSpeed = 0.285f;

     m_pCharacterUsualMove = new CharacterUsualMove(
          pCharacter ,
          params ,
          new AmefootUsualMove_MoveEvent(pCharacter) ,
          new AmefootUsualHitEvent(pCharacter)
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Execute(AmefootPlayer* pCharacter)
{
     Vector2 stickScreen = controller::GetStickValue(controller::stick::left , pCharacter->m_PlayerInfo.number);
     Vector3 stickWorld = DefCamera.GetRight() *stickScreen.x + DefCamera.GetForward()* stickScreen.y;
     stickScreen.x = stickWorld.x;
     stickScreen.y = stickWorld.z;

     // スティックの値をセット
     m_pCharacterUsualMove->SetStickValue(stickScreen);

     // 移動クラス更新
     m_pCharacterUsualMove->Update();
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterUsualMove;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// MoveEvent
//-----------------------------------------------------------------------------------------
AmefootPlayerState_UsualMove::MoveEvent::MoveEvent(AmefootPlayer* pAmefoot) :
     m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::MoveEvent::Update(bool isRun , RATIO speed)
{
     m_pAmefoot->m_Renderer.Update(1);

     // モデルのワールド変換行列を更新
     chr_func::CreateTransMatrix(
          m_pAmefoot ,
          m_pAmefoot->m_ModelSize ,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::MoveEvent::RunStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Run);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::MoveEvent::StandStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Stand);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::MoveEvent::RunEnd()
{}
//-----------------------------------------------------------------------------------------




