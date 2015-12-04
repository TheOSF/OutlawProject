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
	m_ReactionPoint = (int)(frand()*100);
}
CharacterComputerReaction::~CharacterComputerReaction()
{
	delete m_ActionEvent;
}
void CharacterComputerReaction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;
	sp.size = m_Params.BallCounterTec*10.0f;

	CharacterComputerReactionHitEvent hitevent(m_cCharacter);
	//�������[�_�[�W�J
	DefDamageMgr.HitCheckSphere(sp, hitevent);

	Vector3 outvec;
	Vector3 outpos;

	//��������
	if (hitevent.HitDamage && m_ReactionPoint<=m_Params.DangerEscape*100)
	{
		hit = hitevent.hittype;
		pDmg = hitevent.pDmg;
		pDmg->CalcPosVec(m_cCharacter->m_Params.pos, &outpos, &outvec);
		/*outvec.y = 0;
		outvec.x -= sinf(outvec.x - PI / 2);
		outvec.z -= cosf(outvec.z - PI / 2);*/
		m_ActionEvent->Reaction(hit,outvec);
	}
		
}