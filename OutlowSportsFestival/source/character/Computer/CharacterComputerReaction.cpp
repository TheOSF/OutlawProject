#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"
#include "../../Ball/UsualBall.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					    pParent,	//����L�����N�^�̃|�C���^
	const CharacterComputerMove::Param&	param,	
	ActionEvent*						pActionEvent
	) :
	m_pCharacter(pParent), 
    m_Params(param),
    m_pActionEvent(pActionEvent),
    m_DoActionCount(0)
{

}


CharacterComputerReaction::~CharacterComputerReaction()
{
	delete m_pActionEvent;
}


void CharacterComputerReaction::Update()
{
    //���łɌ��m���Ă������ǂ���
    if (m_DoActionCount > 0)
    {
        //��������
        if (--m_DoActionCount == 0)
        {
            m_pActionEvent->Reaction(m_HitType, m_DamageVec);
        }
    }
    else
    {
        //�{�[�����m
        CheckBall();

        //�_���[�W���m
        CheckDamage();

        //�L�����N�^���m
        CheckNearCharacter();
    }
}

bool CharacterComputerReaction::CheckBall()
{
    BallBase* pCounterBall = nullptr;
    Vector3 pos = m_pCharacter->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    Vector3 out;

    //�J�E���^�[�\�ȃ{�[�������o
    if (DefBallMgr.GetCounterBall(
        &pCounterBall,
        pos,
        &out,
        10.0f,
        5,
        m_pCharacter
        ))
    {
        //���o�ł����ꍇ

        //�����t���[���ݒ�
        m_DoActionCount = (int)(m_Params.BallCounterSpeed * 10.0f * frand());

        //�p�����[�^�ݒ�
        m_HitType = CharacterComputerReactionHitEvent::HitType::CanCounter;
        m_DamageVec = pCounterBall->m_Params.move;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckNearCharacter()
{
    CharacterBase* pNearCharacter = nullptr;

    if (chr_func::CalcAtkTarget(m_pCharacter, PI, 3.0f, &pNearCharacter) && m_Params.DangerEscape >= frand())
    {
        //���o�ł����ꍇ

        //�����t���[���ݒ�
        m_DoActionCount = 2;

        //�p�����[�^�ݒ�
        m_HitType = CharacterComputerReactionHitEvent::HitType::Escape;
        m_DamageVec = m_pCharacter->m_Params.pos - pNearCharacter->m_Params.pos;
        m_DamageVec.y = 0;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckDamage()
{
    SphereParam sp;

    sp.pos = m_pCharacter->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    sp.size = 5.0f; 

    CharacterComputerReactionHitEvent hitevent(m_pCharacter);

    //�������[�_�[�W�J
    DefDamageMgr.HitCheckSphere(sp, hitevent);

    //���m�ł��Ă����ꍇ
    if (hitevent.HitDamage && m_Params.DangerEscape >= frand())
    {
        //�����t���[���ݒ�
        m_DoActionCount = 1;

        //�p�����[�^�ݒ�
        Vector3 OutPos = Vector3Zero;
        hitevent.pDmg->CalcPosVec(m_pCharacter->m_Params.pos, &OutPos, &m_DamageVec);

        m_HitType = CharacterComputerReactionHitEvent::HitType::Escape;

        return true;

    }

    return false;
}