#include "AmefootPlayerState_Evasion.h"
#include "../MoveClass/AmefootEvasionStandUpClass.h"
#include "../../CharacterEvasionClass.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"


//-----------------------------------------------------------------------------------------
// EvasionEvent
//-----------------------------------------------------------------------------------------
typedef class AmefootPlayerState_Evasion::EvasionEvent : public CharacterEvasion::Event
{
public:
     EvasionEvent(AmefootPlayer* pAmefoot);

     void Update()override;

     void EvasionStart()override;

     void EvasionEnd()override;

     bool IsEvasionContinue()const override;

private:
     AmefootPlayer* m_pAmefoot;
     int m_timer;

     const int kMaxEvasionFrame = 120;

}AmefootEvasion_Event;



//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Evasion
// [ アメフト ] 回避行動をするステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Evasion::AmefootPlayerState_Evasion() :
     m_pAmefootEvasionStandUpClass(nullptr) ,
     m_pCharacterEvasion(nullptr)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Enter(AmefootPlayer* pCharacter)
{
     {
          CharacterEvasion::EvasionParams params;
          params.AllFrame = 30;
          params.MaxTurnRadian = 0.0f;
          params.MoveDownSpeed = 0.25f;
          params.MoveSpeed = pCharacter->m_Params.move.Length();
          params.NoDamageEndFrame = 30;
          params.NoDamageStartFrame = 8;

          m_pCharacterEvasion = new CharacterEvasion(
               pCharacter,
               new AmefootEvasion_Event(pCharacter),
               params
               );
     }

     {
          AmefootEvasionStandUpClass::Params params;
          params.AllFrame = 8;
          params.MotionNumber = AmefootPlayer::Motion_Dummy_2;

          m_pAmefootEvasionStandUpClass = new AmefootEvasionStandUpClass(
               pCharacter,
               params
               );
     }
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Execute(AmefootPlayer* pCharacter)
{
     DamageManager::HitEventBase NoDamage;
     AmefootUsualHitEvent HitEvent(pCharacter);

     if ( m_pCharacterEvasion->Update() )
     {
          DamageManager::HitEventBase* pDamageEvent = &HitEvent;
          if ( !m_pCharacterEvasion->CanGetDamage() )
          {
               pDamageEvent = &NoDamage;
          }
          chr_func::UpdateAll(pCharacter, pDamageEvent);
          return;
     }

     if ( m_pAmefootEvasionStandUpClass->Update() )
     {
          chr_func::UpdateAll(pCharacter, &HitEvent);
     }
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterEvasion;
     delete m_pAmefootEvasionStandUpClass;
}
//-----------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------
// EvasionEvent
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Evasion::EvasionEvent::EvasionEvent(AmefootPlayer* pAmefoot) :
     m_pAmefoot(pAmefoot) ,
     m_timer(0)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::EvasionEvent::Update()
{
     m_timer++;
     m_pAmefoot->m_Renderer.Update(1);

     // モデルのワールド変換行列を更新
     chr_func::CreateTransMatrix(
          m_pAmefoot ,
          m_pAmefoot->m_ModelSize ,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::EvasionEvent::EvasionStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Dummy_1);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::EvasionEvent::EvasionEnd()
{}
//-----------------------------------------------------------------------------------------
bool AmefootPlayerState_Evasion::EvasionEvent::IsEvasionContinue()const
{
     if ( m_timer <= kMaxEvasionFrame )
     {
          return controller::GetPush(controller::button::batu , m_pAmefoot->m_PlayerInfo.number);
     }
     return false;
}
//-----------------------------------------------------------------------------------------

