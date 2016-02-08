#ifndef __BASEBALL_PLAYER_H__
#define __BASEBALL_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	
#include "../../Ball/UsualBall.h"
#include "BaseballEquip.h"

class BaseballPlayer;
class BaseballEquip;
//�X�e�[�g�}�V���^���t��
typedef ChrStateMachine< BaseballPlayer, CharacterBase::MsgType> BaseballStateMachine;
typedef ChrState< BaseballPlayer, CharacterBase::MsgType>BaseballState;



//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

namespace baseball_player
{
	enum MotionType
    {
		//�@�o�b�^�[
        _mb_Stand_B = 1,				//�@�ҋ@
        _mb_Run_B,				        //�@����
        _mb_Stop_B,				        //�@�����~�܂�
        _mb_Shot_B,                     //�@������
        _mb_Evasion_B = 6,	                //�@���
        _mb_Atk1_B,						//�@�ߋ����U��1
        _mb_Atk2_B = 8,						//�@�ߋ����U��2
        _mb_Atk3_B = 9,						//�@�ߋ����U��3
        _mb_CounterPose_B,			    //�@�J�E���^�[�\��
        _mb_CounterRight_B,			    //�@�J�E���^�[�i�E�j
        _mb_CounterLeft_B,			    //�@�J�E���^�[�i���j
        //    _mb_Change_B,			        //�@�ؑ�(�o�b�^�[���s�b�`���[)
        //  _mb_Change_P,			        //�@�ؑ�(�s�b�`���[���o�b�^�[)
        _mb_SpAtk_B = 14,			        //�@�K�E�Z

        _mb_Damage_Weak_B = 15,			    //�@�_���[�W
        _mb_Damage_Vanish_Fly_B,          //�@������ђ�
        _mb_Damage_Vanish_Down_B,         //�@������ђ��n
        _mb_Damage_Vanish_StandUp_B,      //�@�N���オ��
        _mb_Damage_Vanish_WallHitAndDown_B = 20,     //�@���藎��
        _mb_Damage_Vanish_Bound_B,        //�@�n�ʂŃo�E���h

        _mb_Change_B = 10,
        _mb_VanishHold_B = 22,       //���܂�Ă���

        _mb_WinPose,
        _mb_LosePose,

        //�@�s�b�`���[
        _mb_Stand_P = 2,               //�@�ҋ@
        _mb_Run_P = 4,					    //�@����
        _mb_Stop_P,						//�@�����~�܂�
        _mb_Evasion_P = 7,                  //�@���

        _mb_Atk1_P = 6,                     //�@�ߋ����U��1�[)
        _mb_Atk2_P = 8,
        _mb_Atk3_P = 10,						//�@�ߋ����U��2
        _mb_CounterPose_P,				//�@�J�E���^�[�\��
        _mb_CounterRight_P = 13,				//�@�J�E���^�[�i�E�j
        _mb_CounterLeft_P,				//�@�J�E���^�[�i���j

        _mb_SpAtk_P = 15,			        //�@�K�E�Z

        _mb_Damage_Weak_P = 16,			    //�@�_���[�W
        _mb_Damage_Vanish_Fly_P,          //�@������ђ�
        _mb_Damage_Vanish_Down_P,         //�@������ђ��n
        _mb_Damage_Vanish_StandUp_P,      //�@�N���オ��
        _mb_Damage_Vanish_WallHitAndDown_P = 21,     //�@���藎��
        _mb_Damage_Vanish_Bound_P,        //�@�n�ʂŃo�E���h

        _mb_Change_P = 5,
        _mb_BallCharge = 1,

        _mb_VanishHold_P = 23,       //���܂�Ă���
	};
}

class BaseballPlayer :public CharacterBase
{
private:
	BaseballStateMachine*		m_pStateMachine;
    CharacterRenderer*          m_pBatterModel;
    CharacterRenderer*          m_pPitcherModel;
    bool                        m_BatterFlg; //�@(true:�o�b�^�[,false:����)
public:

    int m_ChargeBallCount;


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

    //���݃t�H�[���̃��f���Ƀ��[�V�������Z�b�g
    void SetMotion(int motion);

    //���݃t�H�[���̃��f�����X�V
    void ModelUpdate(RATIO Speed = 1.0f);

	bool SetState(BaseballState* state, int Important = 0);
	bool Update();
	bool CharacterMsg(MsgType mt);

	//�@�����؂�ւ�
	void ChangeMode();

    //�t�H�[�����w�肵�ăZ�b�g����
    bool SetMode(bool isBatter);

	//�@�Q�b�^�[�E�Z�b�^�[
    inline bool getBatterFlg()const{ return m_BatterFlg; }
    inline bool getPitcherFlg()const{ return !m_BatterFlg; }

    inline bool isBatter()const{ return m_BatterFlg; }
    inline bool isPitcher()const{ return !m_BatterFlg; }

    //���݃t�H�[���̃��f�����Q�b�g
    CharacterRenderer* getNowModeModel();

    void MinusChargeBallCount(int value);

private:
    //�@���Z�b�g
    void Reset();
};

#endif