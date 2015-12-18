#include "TennisPlayer.h"
#include "../../Damage/Damage.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"

/*
enum MotionType
{
    _mt_Stand = 0,
    _mt_Run = 1,

    _mt_Attack1 = 2,
    _mt_Shot = 5,
    _mt_CounterRight = 3,
    _mt_CounterLeft = 4,
    _mt_Smash = 5,
    _mt_Guard = 6,
    _mt_Rolling = 7,
    _mt_BoundSmash = 3,
    _mt_Damage_Weak = 20,

    _mt_Damage_Vanish_Fly = 9,
    _mt_Damage_Vanish_Down = 10,
    _mt_Damage_Vanish_StandUp = 11,

    _mt_WinPose = 13,
    _mt_LosePose = 14,

    _mt_Attack2 = 16,
    _mt_Attack3 = 17,

    _mt_CounterPose = 18,
    _mt_Hakushu = 21,

};

*/


//*************************************************************
//		�e�j�X�v���C���[�N���X
//*************************************************************

TennisPlayer::TennisPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Tennis))),
m_ModelSize(0.055f),
m_DontBoundBallAtkTimer(0)
{
	m_pStateMachine = new TennisStateMachine(this);

    m_Params.maxHP = m_Params.HP = 100; 
}

TennisPlayer::~TennisPlayer()
{
	delete m_pStateMachine;
}

void TennisPlayer::SetState(TennisState* state, int Important)
{
	//�V�����X�e�[�g���Z�b�g
    m_pStateMachine->set_state(state, Important);
}


bool TennisPlayer::Update()
{
	//�X�e�[�g���s
	m_pStateMachine->state_execute();

    if (m_DontBoundBallAtkTimer > 0)
    {
        --m_DontBoundBallAtkTimer;
    }

    //�L�����N�^��{�X�V
    BaseUpdate();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool TennisPlayer::CharacterMsg(MsgType mt)
{
    if (mt == MsgType::_RoundReset)
    {
        SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(this));
        m_Renderer.SetMotion(_mt_Stand);
        m_Renderer.Update(0);
    }

	return m_pStateMachine->Msg(mt);
}

bool TennisPlayer::isCanBoundBallAtk()
{
    return m_DontBoundBallAtkTimer == 0;
}

void TennisPlayer::SetDontBoundBallAtkTimer()
{
    m_DontBoundBallAtkTimer = 5;
}

