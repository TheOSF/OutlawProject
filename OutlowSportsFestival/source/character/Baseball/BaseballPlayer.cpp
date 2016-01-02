#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"

#include "Computer\/BaseballPlayerState_ComMove.h"
#include "BaseballState_Change.h"
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

const BaseballPlayer::SkillParam BaseballPlayer::skillparams =
{
	0.05f,
	0.05f,
	0.05f,
};

//�@�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new  BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Baseball))), batterflg(true),  changetime(20),
helmetEquip(nullptr), capEquip(nullptr), batEquip(nullptr), groveEquip(nullptr), changeflg(false)
{
	m_pStateMachine = new BaseballStateMachine(this);
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));

	//�@�̗͒ቺ(�f�o�b�N�p)
	m_Params.maxHP = m_Params.HP = 100;
	
	temp_batterflg = batterflg;
	helmetEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Helmet);
	batEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Bat);
	
    m_ModelSize = 0.05f;
}

//�@�f�X�g���N�^
BaseballPlayer::~BaseballPlayer()
{
	delete m_pStateMachine;
	
}

//�@�X�e�[�g�Z�b�g
void BaseballPlayer::SetState(BaseballState* state, int Important)
{
	//�@�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state,Important);
}

//�@�X�V
bool BaseballPlayer::Update()
{

	//�@�؂�ւ��\���ԑ���
	changetime++;
	// �X�e�[�g���s
	m_pStateMachine->state_execute();
	//�@�����i�ؑ�
	if (temp_batterflg != batterflg)
	{
		CheangeEquip();
	}
	//�L�����N�^��{�X�V
	BaseUpdate();
	
	return true;	//���true��Ԃ��Ə�������Ȃ�
}


bool  BaseballPlayer::CharacterMsg(MsgType mt)
{
	//�@���E���h���ƂɃ��Z�b�g
	if (mt == MsgType::_RoundReset)
	{
		Riset();
	}

	return m_pStateMachine->Msg(mt);
}

void BaseballPlayer::CheangeEquip()
{
		//�@�o�b�^�[��
		if (batterflg)
		{
			//�@���ݑ������Ă��镨�𕨗�����&nullptr���
			capEquip->ToPhysicMove();
			groveEquip->ToPhysicMove();
			capEquip = nullptr;
			groveEquip = nullptr;
			//�@�V����������new
			helmetEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Helmet);
			batEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Bat);

		}
		else
		{
			//�@���ݑ������Ă��镨�𕨗�����&nullptr���
			helmetEquip->ToPhysicMove();
			batEquip->ToPhysicMove();
			helmetEquip = nullptr;
			batEquip = nullptr;
			//�@�V����������new
			capEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Cap);
			groveEquip = new BaseballEquip(this, 1, BaseballEquip::MeshType::Grove);

		}
		//�@�ꎞ�ۑ��ɍ��̃t���O����
		temp_batterflg = batterflg;
	
}

//�@���Z�b�g
void BaseballPlayer::Riset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	
	m_Renderer.SetMotion(baseball_player::_mb_Stand);
	m_Renderer.Update(0);
	//batterflg = true;
	changetime = 20;

}

