#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"

#include "CharacterComputerReactionHitEvent.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

//---------------------------------------------------//
/*

�v�]�P
�U�����肪�������ɁA�����N���X�ł���
ActionEvent���p�������N���X��Reaction�֐����ĂԁB
�v�]�Q


*/


class CharacterComputerReaction
{
public:
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Reaction(CharacterComputerReactionHitEvent::HitType hittype,Vector3 vec) = 0;			//�U���J�n���ɌĂ΂��֐�
	};
	CharacterComputerReaction(
		CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
		const CharacterComputerMove::Param&	param,		//�ړ��p�����[�^�\����
		ActionEvent*						ActionEvent	//�ړ��C�x���g�ɔ�������N���X
		);

	~CharacterComputerReaction();
	void Update();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	CharacterBase*	                  m_cCharacter;
	ActionEvent*  	                  m_ActionEvent;
	CharacterComputerMove::Param	  m_Params;
	CharacterComputerReactionHitEvent::HitType hit;
	DamageBase* pDmg;
};
