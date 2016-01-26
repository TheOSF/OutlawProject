#include "AmefootPlayerState_ReceiveVanishDamage.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterDamageVanish.h"
#include "../../CharacterFunction.h"
#include "../AmefootUsualHitEvent.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../GameSystem/GameController.h"
#include "AmefootPlayerState_Evasion.h"

//-----------------------------------------------------------------------------------------
// VanishEvent
//-----------------------------------------------------------------------------------------
typedef class AmefootPlayerState_ReceiveVanishDamage::VanishEvent : public CharacterDamageVanish::Event
{
public:
     VanishEvent(AmefootPlayer* pAmefoot);

     void FlyStart()override;

     void Flying(Matrix const& rotate , RATIO speed)override;

     void DownStart()override;

     void Downing()override;

     void StandUpStart()override;

     void StandUping()override;

     void End()override;

     void HitWall()override;

     void HitFloor()override;

     void HitFloorAndStandUp()override;

     void HitWallUpdate()override;

     void CanActionUpdate()override;

private:
     AmefootPlayer* m_pAmefoot;
}AmefootVanishDamage_Event;
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ReceiveVanishDamage
// [ アメフト ] 吹き飛びダメージを受けるステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ReceiveVanishDamage::AmefootPlayerState_ReceiveVanishDamage(
     Vector3 const& damageVec
     ) :
     m_damageVec(damageVec)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::Enter(AmefootPlayer* pCharacter)
{
     // パラメータ設定
     CharacterDamageVanish::Param params;
     params.rotate_speed = Vector3(0.0f , 0.0f , 0.0);
     params.move = m_damageVec;
     params.down_frame = 18;
     params.standup_frame = 60;

     // 吹き飛びクラス作成
     m_pCharacterDamageVanish = new CharacterDamageVanish(
          pCharacter ,
          params,
          new AmefootVanishDamage_Event(pCharacter) ,
          new AmefootUsualHitEvent(pCharacter)
          );

     //ヒットエフェクト作成
     EffectFactory::VanishEffect(
         pCharacter,
         m_damageVec
         );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::Execute(AmefootPlayer* pCharacter)
{
     m_pCharacterDamageVanish->Update();
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterDamageVanish;
}

void AmefootPlayerState_ReceiveVanishDamage::StateMachineExit(AmefootPlayer* pCharacter)
{
    Exit(pCharacter);
}

//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// VanishEvent
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ReceiveVanishDamage::VanishEvent::VanishEvent(AmefootPlayer* pAmefoot):
     m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::FlyStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Start);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::Flying(Matrix const& rotation,RATIO speed)
{
     m_pAmefoot->m_Renderer.Update(1);

     chr_func::CreateTransMatrix(
          m_pAmefoot , 
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );

     m_pAmefoot->m_Renderer.m_TransMatrix = rotation*m_pAmefoot->m_Renderer.m_TransMatrix;
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::DownStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Landing);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::Downing()
{
     m_pAmefoot->m_Renderer.Update(1);

     chr_func::CreateTransMatrix(
          m_pAmefoot ,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::StandUpStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Standup);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::StandUping()
{
     m_pAmefoot->m_Renderer.Update(1);

     chr_func::CreateTransMatrix(
          m_pAmefoot ,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::End()
{
    m_pAmefoot->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pAmefoot));
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::HitWall()
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Fall);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::HitFloor()
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Landing);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::HitFloorAndStandUp()
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Standup);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::HitWallUpdate()
{
    m_pAmefoot->m_Renderer.Update(1);

    chr_func::CreateTransMatrix(
        m_pAmefoot,
        &m_pAmefoot->m_Renderer.m_TransMatrix
        );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveVanishDamage::VanishEvent::CanActionUpdate()
{
    if (m_pAmefoot->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        if (controller::GetPush(controller::button::batu, m_pAmefoot->m_PlayerInfo.number))
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        }
    }
    else
    {
        if (frand() < 0.01f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        }
    }
}
//-----------------------------------------------------------------------------------------
