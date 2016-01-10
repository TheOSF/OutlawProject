#include "Baseball_HitEvent.h"
#include "BaseballplayerState_DamageMotionWeak.h"
#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState_DamageMotion_Die.h"
#include "BaseballPlayerState_Controll_Vanish.h"
#include "../CharacterFunction.h"



#include "../CharacterHitEventFunc.h"  //�Ђ�ݕ���֐����g�p���邽��



BaseballHitEvent::BaseballHitEvent(BaseballPlayer* pt,bool CounterHit) :
m_pBaseball(pt),
m_CounterHit(CounterHit)
{

}

bool BaseballHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //�Ђ�ݕ���
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pBaseball, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //���S�X�e�[�g��
        m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //��Ђ�݃X�e�[�g��
        m_pBaseball->SetState(new BaseballState_DamageMotion_Weak(m_pBaseball, DamageVec, m_CounterHit,pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //������уX�e�[�g��
        m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, DamageVec), 1);
        break;

    case CharacterHitEventFunc::SetType::Controll_Hit:
    {
        //�R���g���[��������уX�e�[�g��
        BaseballState_ControllVanish* p = new BaseballState_ControllVanish(m_pBaseball);
        m_pBaseball->SetState(p, 2);
        ((DamageControllVanish*)pDmg)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
    }
        break;
        //

    case CharacterHitEventFunc::SetType::_None:
        //�������Ȃ�(���g�̃_���[�W�������ꍇ
        return false;

    default:



        MyAssert(false, "�Ђ�ݕ��򂪂ł��Ă��܂���");
        return false;
    }


    return true;
}

