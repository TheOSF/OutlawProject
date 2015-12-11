#pragma once

#include "BaseballPlayer.h"
#include "../../Render/LightObject.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	BaseballState_SPAttack_B(BaseballPlayer* b);
	~BaseballState_SPAttack_B();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	void(BaseballState_SPAttack_B::*m_pStateFunc)();

	int                     m_Timer;
	bool					timeflg;
	BaseballPlayer*         m_pBaseBall;
	DamageShpere            m_Damage;
	UINT                    m_ChrLiveCount;
	PointLight              m_Light;

	
	//void State_Atk1();			 //�@����
	//void State_Atk1End();		 //�@�����O�ꂽ
	void State_Atk2();			 //�@2����

	void State_Finish()
	{
		//�@���G�t�F�N�g����
		ThunderInvoke(8);
	}		 //�@�K�E�I���

	void FreezeGame(UINT frame); //�@The World

	void ThunderInvoke(UINT point_num);
};

