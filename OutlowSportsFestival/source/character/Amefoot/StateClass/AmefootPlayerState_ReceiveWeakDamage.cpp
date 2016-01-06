#include "AmefootPlayerState_ReceiveWeakDamage.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterDamageMotion.h"
#include "../../CharacterFunction.h"
#include "../../../Effect/HitEffectObject.h"
#include "../../../Effect/BlurImpact.h"
#include "../AmefootUsualHitEvent.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../../../Effect/EffectFactory.h"


//-----------------------------------------------------------------------------------------
// MotionEvent
//-----------------------------------------------------------------------------------------
typedef class AmefootPlayerState_ReceiveWeakDamage::MotionEvent : public CharacterDamageMotion::Event
{
public:
     MotionEvent(AmefootPlayer* pAmefoot);

     void Update(RATIO speed)override;

     void Start()override;

     void End()override;

     void SetLight(RATIO power)override;

private:
     AmefootPlayer* m_pAmefoot;
}AmefootWeakDamage_MotionEvent;
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ReceiveWeakDamage
// [ アメフト ] 弱いダメージを受けるステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ReceiveWeakDamage::AmefootPlayerState_ReceiveWeakDamage(Vector3 const& damageVec) :
     m_pCharacterDamageMotion(nullptr) ,
     m_damageVec(damageVec)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::Enter(AmefootPlayer* pCharacter)
{
     // パラメータ設定
     CharacterDamageMotion::Params params;
     params.damage_vec = m_damageVec;

     // ダメージモーションクラス作成
     m_pCharacterDamageMotion = new CharacterDamageMotion(
          pCharacter ,
          new AmefootWeakDamage_MotionEvent(pCharacter) ,
          new AmefootUsualHitEvent(pCharacter),
          params
          );

     //エフェクト
     EffectFactory::HitEffect(
         pCharacter,
         m_damageVec
         );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::Execute(AmefootPlayer* pCharacter)
{
     // 更新
     m_pCharacterDamageMotion->Update();
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterDamageMotion;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// MotionEvent
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ReceiveWeakDamage::MotionEvent::MotionEvent(AmefootPlayer* pAmefoot) :
     m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::MotionEvent::Update(RATIO speed)
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
void AmefootPlayerState_ReceiveWeakDamage::MotionEvent::Start()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Weak);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::MotionEvent::End()
{
     m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::MotionEvent::SetLight(RATIO power)
{}
//-----------------------------------------------------------------------------------------

