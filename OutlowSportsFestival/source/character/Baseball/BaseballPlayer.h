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
		_mt_Damage_Weak,

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
	int num;       //�@�^�[�Q�b�g�i���o�[
protected:
	//�@����p�p
	Vector3 vf;
	Vector3 vt;
	float cross ;
	float dot;
	//�@�����֌W
	Vector3 v;
	float len;
	float templen;
	//�@pos�ꎞ�ۑ��p
	Vector3 temppos;
	//�@target��pos
	Vector3 target;
	//�@�z�[�~���O�֌W
	D3DXQUATERNION q, invq, qpos;
	Vector3 v1, v2, axis;
	BallBase::Params tempparam;

private:
	BaseballStateMachine*		m_pStateMachine;
	float acc;//�@�����x
public:


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	CharacterRenderer	m_Renderer;
	const float m_ModelSize;


	void SetState(BaseballState* state);
	bool Update();
	bool Msg(MsgType mt);
public:
	//�@�؂�ւ�
	void Change();
	//�@�������֐���
	BallBase::Params BaseballShot(BaseballPlayer* b, BallBase::Params p, float speed);
	//�@����p���v�Z
	Vector3 AngleField(Vector3 t, float range);
	//�@�������^�[�Q�b�g�I��
	BallBase::Params TargetDecision( BallBase::Params p, Vector3 t);
	//�@�z�[�~���O�v�Z
	BallBase::Params Homing(BallBase::Params p, Vector3 t);
	

public:
	//�@�Q�b�^�[
	bool getBatterFlg(){ return batterflg; }
	int getCharacounter(){ return characounter; }
	int getOutcounter(){ return outcounter; }
	int getNum(){ return num; }
public:	   
	//�@�Z�b�^�[
	void setAcc(float a){ acc = a; }//�@�����x
	void setCharacounter(int num){ characounter = num; }
	void setOutcounter(int num){ outcounter = num; }
public:

};
#endif