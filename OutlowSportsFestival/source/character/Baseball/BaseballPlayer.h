#ifndef __BASEBALL_PLAYER_H__
#define __BASEBALL_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	
#include "../../Ball/UsualBall.h"

class BaseballPlayer;

//�X�e�[�g�}�V���^���t��
typedef ChrStateMachine< BaseballPlayer, CharacterBase::MsgType> BaseballStateMachine;
typedef ChrState< BaseballPlayer, CharacterBase::MsgType>BaseballState;

//�@�ő�����x
#define MaxAcceleration 0.8f

//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

namespace baseball_player{
	enum MotionType{
		_mt_Stand,
		_mt_Run,
		_mt_Shot,
		_mt_CounterPose,
		_mt_Counter,
		_mt_Evasion = 5,  // ���
	};

	enum PositionMode
	{
		_pm_Butter,
		_pm_Pitcher,
	};
}

class BaseballPlayer :public CharacterBase
{
protected:
	bool batterflg;//�@(true:�o�b�^�[,false:����)
	int characounter;//�@�o���L������
	int outcounter;//�@����p�O,����ł鐔

private:
	BaseballStateMachine*		m_pStateMachine;
	float acc;//�@�����x
public:


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	CharacterRenderer	m_Renderer;


	void SetState(BaseballState* state);
	bool Update();
	bool Msg(MsgType mt);
public:
	//�@�؂�ւ�
	void Change();
	//�@�������֐���
	BallBase::Params BaseballShot(BaseballPlayer* b, BallBase::Params p);
	//�@�������^�[�Q�b�g�I��
	BallBase::Params TargetDecision( BallBase::Params p, Vector3 t);
public:
	//�@�Q�b�^�[
	bool getBatterFlg(){ return batterflg; }

public:
	//�@�Z�b�^�[
	void setAcc(float a){ acc = a; }//�@�����x
public:

};
#endif