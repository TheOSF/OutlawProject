#include "GameController.h"
#include "../../Input/GamePad/GamePadManager.h"

static const GamePadIndex button_key_code[]=
{
	GamePadIndex_RightPad_Right,		// ��
	GamePadIndex_RightPad_Down,		// �~
	GamePadIndex_RightPad_Left,			// ��
	GamePadIndex_RightPad_Up,			// ��

	GamePadIndex_RightShoulder1,			// R1
	GamePadIndex_RightShoulder2,			// R2
	GamePadIndex_RightStick_Button,	// R3

	GamePadIndex_LeftShoulder1,			// L1
	GamePadIndex_LeftShoulder2,			// L2
	GamePadIndex_LeftStick_Button,		// L3

	GamePadIndex_LeftPad_Up,				// ��
	GamePadIndex_LeftPad_Down,			// ��
	GamePadIndex_LeftPad_Right,			// ��
	GamePadIndex_LeftPad_Left,				// ��
};


static const GamePadIndex stick_key_code[][2]=
{
	{	GamePadIndex_LeftStick_X,	GamePadIndex_LeftStick_Y },	// ���X�e�B�b�N
	{ GamePadIndex_RightStick_X, GamePadIndex_RightStick_Y },	// �E�X�e�B�b�N
};


// �������u�Ԃ��ǂ���
bool controller::GetTRG(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) == 3;
}

bool controller::GetLeave(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) == 2;
}

// �����Ă��邩�ǂ���
bool controller::GetPush(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) != 0;
}

// �R���g���[���̏�Ԏ擾
controller::button::button_state controller::GetButtonState(controller::button::button_type x, CONTROLLER_NUM num)
{
	return (controller::button::button_state)(GamePadManager::GetState(num, button_key_code[x]));
}

// �X�e�B�b�N�̌X���̒l�𓾂�
Vector2 controller::GetStickValue(stick::stick_type x, CONTROLLER_NUM num)
{
	Vector2 ret(
		GamePadManager::GetState(num, stick_key_code[x][0])*0.001f,
		GamePadManager::GetState(num, stick_key_code[x][1])*-0.001f
		);
	
	//�␳
	if (fabsf(ret.x)<0.3f)ret.x = 0;
	if (fabsf(ret.y)<0.3f)ret.y = 0;

	if (Vector2Length(ret) > 1)return Vector2Normalize(ret);

	return ret;
}


// �w�肵���R���g���[���[��U��������
void controller::SetVibration(DWORD power, float second,  CONTROLLER_NUM num, int pattern)
{
    GamePadManager::Vibration(num, pattern, (DWORD)(second*(float)DI_SECONDS), power);
}
