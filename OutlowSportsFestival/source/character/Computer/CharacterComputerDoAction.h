#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

class CharacterBase;

//--------------------------------------------------//
//  CharacterComputerDoAction�N���X�͓����N���X�ł���
//  ActionEvent�N���X��Attack�֐���
//�@�R���s���[�^���U������ׂ����ɌĂԃN���X�ł���
//--------------------------------------------------//

class CharacterComputerDoAction
{
public:
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Attack(float len) = 0;			//�U���J�n���ɌĂ΂��֐�
	};
	CharacterComputerDoAction(
		CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
		const CharacterComputerMove::Param&	param,		//�ړ��p�����[�^�\����
		ActionEvent*						ActionEvent,	//�ړ��C�x���g�ɔ�������N���X
		DamageManager::HitEventBase*	pHitEventBase//�_���[�W���󂯂����ɔ�������N���X
		);

	~CharacterComputerDoAction();
	void Update();
	float GetMoveTargetLength();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	CharacterBase*	                  m_cCharacter;
	ActionEvent*  	                  m_ActionEvent;
	HitEventBase*	                  m_pHitEventBase;
	CharacterComputerMove::Param	  m_Params;
};