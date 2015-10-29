#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
	const CharacterComputerMove::Param&	param,		//�ړ��p�����[�^�\����
	ActionEvent*						pActionEvent,	//�ړ��C�x���g�ɔ�������N���X
	DamageManager::HitEventBase*	pHitEventBase//�_���[�W���󂯂����ɔ�������N���X
	) :
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent), m_pHitEventBase(pHitEventBase)
{


}
CharacterComputerReaction::~CharacterComputerReaction()
{

}
void CharacterComputerReaction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;
	//�������[�_�[�W�J
	//DefDamageMgr.HitCheckSphere(sp, m_pHitEventBase->Hit());


	//��������
	m_ActionEvent->Reaction();
		
}