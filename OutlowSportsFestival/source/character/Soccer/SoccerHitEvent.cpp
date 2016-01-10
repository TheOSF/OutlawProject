#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
#include "../CharacterFunction.h"

#include "../CharacterHitEventFunc.h"  //�Ђ�ݕ���֐����g�p���邽��

#include "SoccerState_ControllVanish.h"


SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps, bool CounterHit) :
m_pSoccer(ps),
m_CounterHit(CounterHit)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //�Ђ�ݕ���
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pSoccer, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //���S�X�e�[�g��
        m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //��Ђ�݃X�e�[�g��
        m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, DamageVec, m_CounterHit, pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //������уX�e�[�g��
        m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, DamageVec), 1);
        break;

    case CharacterHitEventFunc::SetType::Controll_Hit:   
    {
        //�R���g���[��������уX�e�[�g��
        SoccerState_ControllVanish* p=new SoccerState_ControllVanish(m_pSoccer);
        m_pSoccer->SetState(p, 2);
        ((DamageControllVanish*)pDmg)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
    }
        break;

    case CharacterHitEventFunc::SetType::_None:
        //�������Ȃ�(���g�̃_���[�W�������ꍇ
        return false;

    default:
        MyAssert(false, "�Ђ�ݕ��򂪂ł��Ă��܂���");
        return false;
    }


    return true;
}
