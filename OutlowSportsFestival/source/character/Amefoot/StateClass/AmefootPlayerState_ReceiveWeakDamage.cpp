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
// [ �A���t�g ] �ア�_���[�W���󂯂�X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ReceiveWeakDamage::AmefootPlayerState_ReceiveWeakDamage(Vector3 const& damageVec) :
     m_pCharacterDamageMotion(nullptr) ,
     m_damageVec(damageVec)
{}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::Enter(AmefootPlayer* pCharacter)
{
     // �p�����[�^�ݒ�
     CharacterDamageMotion::Params params;
     params.damage_vec = m_damageVec;

     // �_���[�W���[�V�����N���X�쐬
     m_pCharacterDamageMotion = new CharacterDamageMotion(
          pCharacter ,
          new AmefootWeakDamage_MotionEvent(pCharacter) ,
          new AmefootUsualHitEvent(pCharacter),
          params
          );

     //�G�t�F�N�g
     EffectFactory::HitEffect(
         pCharacter,
         m_damageVec
         );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ReceiveWeakDamage::Execute(AmefootPlayer* pCharacter)
{
     // �X�V
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

     // ���f���̃��[���h�ϊ��s����X�V
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

