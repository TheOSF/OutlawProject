#include "BaseballPlayer.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

//�@�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(false),  m_ModelSize(0.05f),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);
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

bool  BaseballPlayer::Msg(MsgType mt){
	return m_pStateMachine->Msg(mt);
}

//�@�؂�ւ�
void BaseballPlayer::Change(){
	if (controller::GetTRG(controller::button::_L1, m_PlayerInfo.number)){
		//�@�G�t�F�N�g
		EffectFactory::Counter(Vector3(m_Params.pos.x, m_Params.pos.y + 2.0f, m_Params.pos.z), 10.0f);
		if (batterflg){
			batterflg = false;
		}
		else{
			batterflg = true;
		}
	}

}
