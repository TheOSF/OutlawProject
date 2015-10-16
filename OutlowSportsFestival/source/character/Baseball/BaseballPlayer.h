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



//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

namespace baseball_player{
	enum MotionType{
		_mb_Stand,
		_mb_Run,
		_mb_Shot,
		_mb_CounterPose=17,
		_mb_CounterRight=3,
		_mb_CounterLeft=3,
		_mb_LosePose,
		_mb_Evasion = 5,  // ���
		_mb_Damage_Weak,
		_mb_Damage_Vanish_Fly,
		_mb_Damage_Vanish_Down,
		_mb_Damage_Vanish_StandUp,
		_mb_Atk1=15,//�@�ߋ����U��
		_mb_Atk2=2,
		_mb_Atk3=16


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
	int changetime;
private:
	BaseballStateMachine*		m_pStateMachine;
	
public:


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	CharacterRenderer	m_Renderer;
	const float m_ModelSize;


	void SetState(BaseballState* state);
	bool Update();
	bool Msg(MsgType mt);
public:

	//�@���Z�b�g
	void Riset();
	
public:
	//�@�Q�b�^�[
	bool getBatterFlg(){ return batterflg; }
	int getChangeTime(){ return changetime; }
	void setChangeTime(int t){ changetime = t; }
	void setBatterFlg(bool flg){ batterflg = flg; }

public:

};
#endif