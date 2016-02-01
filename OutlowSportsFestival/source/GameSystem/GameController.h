#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "iextreme.h"

//***********************************************
//	�R���g���[���[�Ɋւ���w�b�_
//***********************************************

//�O���錾����
struct ControllerButtonSet;

namespace controller
{
	typedef int CONTROLLER_NUM;	//�R���g���[���ԍ�


    enum :CONTROLLER_NUM
    {
        __ERROR_CONTROLLER_NUM = -1  //�R���g���[���ԍ��̃G���[�n�l
    };

	namespace button
	{
		enum button_type
		{
			//�{�^���^�C�v
			maru,
			batu,
			shikaku,
			sankaku,

			//RL�{�^��
			_R1,
			_R2,
			_R3,
			_L1,
			_L2,
			_L3,

			//�����L�[�^�C�v
			up,
			down,
			right,
			left,

			max,//�{�^���^�C�v�ő吔
		};

		enum button_state
		{
			bs_up,
			bs_down,
			bs_trg,
		};
	}
	namespace stick{
		enum stick_type
		{
			//�X�e�B�b�N�^�C�v
			left,
			right,

			max//�X�e�B�b�N�^�C�v�ő吔
		};
	}
	//�������u�Ԃ��ǂ���
    bool	GetTRG(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);
	//�������u�Ԃ��ǂ���
	bool	GetLeave(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);
	//�����Ă��邩�ǂ���
	bool	GetPush(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);

    //�����Ă��邩�ǂ���(�N����)
    bool	GetPushAnyController(button::button_type x);

    //�������u�Ԃ��ǂ���(�N����)
    bool	GetTRGAnyController(button::button_type x);

    //�����̒l���X�e�B�b�N�̌덷�̒l�ȓ����ǂ���
    bool    isStickAdjustValue(float value);

	// �R���g���[���̏�Ԏ擾
	button::button_state GetButtonState(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);

    //�X�e�B�b�N(�܂��̓J�[�\��)��|�����u�Ԃ��ǂ����𓾂�
    bool GetStickJustMove(stick::stick_type x, CONTROLLER_NUM num);

	//�X�e�B�b�N(�܂��̓J�[�\��)�̌X���̒l�𓾂�
	Vector2 GetStickValue(stick::stick_type x, CONTROLLER_NUM num);

	//�w�肵���R���g���[���[��U��������
    void SetVibration(
        DWORD power,       //-10000�`10000
        float second,
        CONTROLLER_NUM num,
        int pattern = 0     //�Ƃ肠�����O�ł���
        );


    //�X�V(�S�̂łP�t���[���Ɉ��Ăяo�������A�g��Ȃ��ŁI)
    void UpdateInfo();
}

//�f�t�H���g��button�Z�b�g��ݒ肷��
void SetUsualButtonSet(ControllerButtonSet& out, controller::CONTROLLER_NUM number);


#endif