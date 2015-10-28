#include "BaseballState_Change.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"

//-------------�؂�ւ��X�e�[�g�N���X-------------

BaseballState_Change::BaseballState_Change():
batterflg(false)
{

}


BaseballState_Change::~BaseballState_Change()
{

}

// �X�e�[�g�J�n
void  BaseballState_Change::Enter(BaseballPlayer* b)
{
	
}


// �X�e�[�g���s
void BaseballState_Change::Execute(BaseballPlayer* b)
{
	batterflg = b->getBatterFlg();
	if (b->getChangeTime() >= 20)
	{
		//�@�G�t�F�N�g
		EffectFactory::Counter(Vector3(b->m_Params.pos.x, b->m_Params.pos.y + 2.0f, b->m_Params.pos.z), 10.0f);
		//�@���ʉ�
		Sound::Play(Sound::Change);
		if (batterflg)
		{
			b->setBatterFlg(false);
		}
		else{
			b->setBatterFlg(true);
		}
		//�@���Z�b�g
		b->setChangeTime(0);
	}

	b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
}

// �X�e�[�g�I��
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}