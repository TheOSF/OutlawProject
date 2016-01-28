#include "BaseballState_Change.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"

//-------------�؂�ւ��X�e�[�g�N���X-------------

BaseballState_Change::BaseballState_Change() :
batterflg(false), m_Timer(0)
{

}


BaseballState_Change::~BaseballState_Change()
{

}

// �X�e�[�g�J�n
void  BaseballState_Change::Enter(BaseballPlayer* b)
{
	m_pBaseball = b;
}


// �X�e�[�g���s
void BaseballState_Change::Execute(BaseballPlayer* b)
{
    ++m_Timer;

    chr_func::XZMoveDown(b, 0.05f);

    if (m_Timer == 1)
    {
        //�@�G�t�F�N�g
        EffectFactory::Change(Vector3(m_pBaseball->m_Params.pos.x, m_pBaseball->m_Params.pos.y + 2.0f, m_pBaseball->m_Params.pos.z), 10.0f);
        //�@���ʉ�
        Sound::Play(Sound::Change);

        b->ChangeMode();
    }

	
	chr_func::UpdateAll(b, &BaseballHitEvent(b));
    b->ModelUpdate();
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
}

// �X�e�[�g�I��
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}

