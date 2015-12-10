#include "Tennis_HitEvent.h"
#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "../CharacterFunction.h"



#include "../CharacterHitEventFunc.h"  //�Ђ�ݕ���֐����g�p���邽��


TennisHitEvent::TennisHitEvent(TennisPlayer* pt, bool CounterHit) :
m_pTennis(pt),
m_CounterHit(CounterHit)
{

}

bool TennisHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //�Ђ�ݕ���
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pTennis, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //���S�X�e�[�g��
        m_pTennis->SetState(new TennisState_DamageMotion_Die(m_pTennis, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //��Ђ�݃X�e�[�g��
        m_pTennis->SetState(new TennisState_DamageMotion_Weak(m_pTennis, DamageVec, m_CounterHit, pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //������уX�e�[�g��
        m_pTennis->SetState(new TennisState_DamageVanish(m_pTennis, DamageVec), 1);
        break;



    case CharacterHitEventFunc::SetType::_None:
        //�������Ȃ�(���g�̃_���[�W�������ꍇ
        return false;

    default:
        MyAssert(false,"�Ђ�ݕ��򂪂ł��Ă��܂���");
        return false;
    }

    
	return true;
}
