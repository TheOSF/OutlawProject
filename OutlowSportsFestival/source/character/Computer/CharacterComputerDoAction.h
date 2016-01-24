#pragma once
#include "../CharacterBase.h"
#include "CharacterComputerMove.h"

class CharacterBase;

//--------------------------------------------------//
//  CharacterComputerDoAction�N���X�͓����N���X�ł���
//  ActionEvent�N���X��Attack�֐���
//�@�R���s���[�^���U������ׂ����ɌĂԃN���X�ł���
//--------------------------------------------------//

class CharacterComputerDoAction
{
public:

	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
        virtual bool Action(CharacterBase* pTarget, float len) = 0;			//�U���J�n���ɌĂ΂��֐�
	};

	CharacterComputerDoAction(
		CharacterBase*			pParent,	            //����L�����N�^�̃|�C���^
        CharacterComputerMove*  pCharacterComputerMove, //AI�ړ��N���X�ւ̃|�C���^
		ActionEvent*			pActionEvent	        //�ړ��C�x���g�ɔ�������N���X
		);

	~CharacterComputerDoAction();

	void Update();

private:
    CharacterComputerMove* const    m_pCharacterComputerMove;
    CharacterBase* const            m_pChr;
	ActionEvent* const              m_pActionEvent;


    bool                 m_DoAttack;

    bool isDoAttack();
};