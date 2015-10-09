#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

//�@�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(false), m_ModelSize(0.05f), changetime(18),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);

	//�@�̗͒ቺ(�f�o�b�N�p)
	m_Params.maxHP = m_Params.HP = 10;
}

//�@�f�X�g���N�^
BaseballPlayer::~BaseballPlayer(){
	delete m_pStateMachine;
}

//�@�X�e�[�g�Z�b�g
void BaseballPlayer::SetState(BaseballState* state){
	//�@�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state);
}

//�@�X�V
bool BaseballPlayer::Update(){
	//�@�؂�ւ�
	Change();
	// �X�e�[�g���s
	m_pStateMachine->state_execute();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool  BaseballPlayer::Msg(MsgType mt)
{
	//�@���E���h���ƂɃ��Z�b�g
	if (mt == MsgType::_RoundReset)
	{
		Riset();
	}

	return m_pStateMachine->Msg(mt);
}

//�@�؂�ւ�
void BaseballPlayer::Change()
{
	//�@�؂�ւ��\���ԑ���
	changetime++;
	if (controller::GetTRG(controller::button::_L1, m_PlayerInfo.number)){
		if (changetime >= 18)
		{
			//�@�G�t�F�N�g
			EffectFactory::Counter(Vector3(m_Params.pos.x, m_Params.pos.y + 2.0f, m_Params.pos.z), 10.0f);
			//�@���ʉ�
			Sound::Play(Sound::Change);
			if (batterflg){
				batterflg = false;
			}
			else{
				batterflg = true;
			}
			//�@���Z�b�g
			changetime = 0;
		}
	}

}


//�@���Z�b�g
void BaseballPlayer::Riset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	m_Renderer.SetMotion(baseball_player::_mb_Stand);
	m_Renderer.Update(0);
	ResetRound();
	batterflg = true;
	changetime = 30;

}