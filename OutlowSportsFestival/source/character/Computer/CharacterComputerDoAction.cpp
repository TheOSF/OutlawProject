#include "CharacterComputerDoAction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerDoAction::CharacterComputerDoAction(
	CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
	const CharacterComputerMove::Param&	param,		//�ړ��p�����[�^�\����
	ActionEvent*						pActionEvent,	//�ړ��C�x���g�ɔ�������N���X
	DamageManager::HitEventBase*	pHitEventBase//�_���[�W���󂯂����ɔ�������N���X
	) :
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent), m_pHitEventBase(pHitEventBase)
{


}
CharacterComputerDoAction::~CharacterComputerDoAction()
{
	delete m_ActionEvent;
	delete m_pHitEventBase;
}
void CharacterComputerDoAction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;



	//�U�����鎞�Ȃ��
	if (rand() % 10 == 0)
	{
		//�����𑪂�A�U������
		m_ActionEvent->Attack(GetMoveTargetLength());

	}
}

float CharacterComputerDoAction::GetMoveTargetLength()
{
	Vector3 v1, v2;
	const float AngleRange = PI / 4;
	Vector3 ret;
	float MostNear = 10000.0f;

	CharacterBase* pTarget = nullptr;

	struct TargetInfo
	{
		bool      ok;
		Vector3   pos;
	};

	TargetInfo targets[8];

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

	for (auto it = ChrMap.begin(); it != ChrMap.end(); ++it)
	{

		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
		{
			continue;
		}
		//�@����p�v�Z
		chr_func::GetFront(m_cCharacter, &v1);

		v2 = it->first->m_Params.pos - m_cCharacter->m_Params.pos;
		v2.y = 0;

		//�p�x�O�Ȃ�K���Ă��Ȃ�
		if (Vector3Radian(v1, v2) > AngleRange)
		{
			continue;
		}

		float len = Vector3Distance(m_cCharacter->m_Params.pos, it->first->m_Params.pos);
		//�ł��������߂��G���^�[�Q�b�g��
		if (len < MostNear)
		{
			pTarget = it->first;
			MostNear = len;
		}
	}
	//MostNear�̕ω��Ȃ��Ȃ�0�x�N�Ԃ�
	if (MostNear >= 9990.0f)
	{
		return 10000;
	}
	return MostNear;
}