#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
	const CharacterComputerMove::Param&	param,		//�ړ��p�����[�^�\����
	ActionEvent*						pActionEvent	//�ړ��C�x���g�ɔ�������N���X
	) :
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent)
{
}
CharacterComputerReaction::~CharacterComputerReaction()
{
	delete m_ActionEvent;
}
void CharacterComputerReaction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;
	sp.size = 10.0f;

	CharacterComputerReactionHitEvent hitevent(m_cCharacter);
	//�������[�_�[�W�J
	DefDamageMgr.HitCheckSphere(sp, hitevent);

	Vector3 outvec(1,1,1);
	Vector3 outpos;

	//��������
	if (hitevent.HitDamage)
	{
		hit = hitevent.hittype;
		//pDmg->CalcPosVec(m_cCharacter->m_Params.pos, &outpos, &outvec);
		m_ActionEvent->Reaction(hit,outvec);
	}
		
}