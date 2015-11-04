#include "GameController.h"




#ifdef OUTLAW2_CONTROLL_TYPE_KEY_BORD



static const KEYCODE button_key_code[] =
{
    KEY_A,		//○
    KEY_B,		//×
    KEY_D,		//□
    KEY_C,		//△

    KEY_R1,		//R1
    KEY_R2,		//R2
    KEY_R3,		//R3

    KEY_L1,		//L1
    KEY_L2,		//L2
    KEY_L3,		//L3

    KEY_UP,		//↑
    KEY_DOWN,	//↓
    KEY_RIGHT,	//→
    KEY_LEFT,	//←
};

static const KEYCODE stick_key_code[][2]=
{
    {	KEY_AXISX,	KEY_AXISY },	//左スティック
    {	KEY_AXISX2, KEY_AXISY2 },	//右スティック
};

//押した瞬間かどうか
bool controller::GetTRG(button::button_type x, CONTROLLER_NUM num)
{
    return KEY(button_key_code[x], num) == 3;
}

bool controller::GetLeave(button::button_type x, CONTROLLER_NUM num)
{
    return KEY(button_key_code[x], num) == 2;
}

//押しているかどうか
bool controller::GetPush(button::button_type x, CONTROLLER_NUM num)
{
    return KEY(button_key_code[x], num) != 0;
}

// コントローラの状態取得
controller::button::button_state controller::GetButtonState(controller::button::button_type x, CONTROLLER_NUM num)
{
    switch (KEY(button_key_code[x], num))
    {
    case 0: return controller::button::button_state::bs_up;
    case 3: return controller::button::button_state::bs_trg;

    default:
        return controller::button::button_state::bs_down;
    }

    return controller::button::button_state::bs_up;
}

//スティックの傾きの値を得る
Vector2 controller::GetStickValue(stick::stick_type x, CONTROLLER_NUM num)
{
    Vector2 ret(KEY(stick_key_code[x][0],num)*0.001f, KEY(stick_key_code[x][1],num)*-0.001f);

    //補正
    if (fabsf(ret.x)<0.3f)ret.x = 0;
    if (fabsf(ret.y)<0.3f)ret.y = 0;

    if (Vector2Length(ret) > 1)return Vector2Normalize(ret);

    return ret;
}

//指定したコントローラーを振動させる
void controller::SetVibration(DWORD power, float second, CONTROLLER_NUM num, int pattern)
{
    //キーボードを振動…ゴクリ
}



#else

#include "../../Input/GamePad/GamePadManager.h"

static const GamePadIndex button_key_code[]=
{
	GamePadIndex_RightPad_Right,		// ○
	GamePadIndex_RightPad_Down,		// ×
	GamePadIndex_RightPad_Left,			// □
	GamePadIndex_RightPad_Up,			// △

	GamePadIndex_RightShoulder1,			// R1
	GamePadIndex_RightShoulder2,			// R2
	GamePadIndex_RightStick_Button,	// R3

	GamePadIndex_LeftShoulder1,			// L1
	GamePadIndex_LeftShoulder2,			// L2
	GamePadIndex_LeftStick_Button,		// L3

	GamePadIndex_LeftPad_Up,				// ↑
	GamePadIndex_LeftPad_Down,			// ↓
	GamePadIndex_LeftPad_Right,			// →
	GamePadIndex_LeftPad_Left,				// ←
};


static const GamePadIndex stick_key_code[][2]=
{
	{	GamePadIndex_LeftStick_X,	GamePadIndex_LeftStick_Y },	// 左スティック
	{ GamePadIndex_RightStick_X, GamePadIndex_RightStick_Y },	// 右スティック
};


// 押した瞬間かどうか
bool controller::GetTRG(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) == 3;
}

bool controller::GetLeave(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) == 2;
}

// 押しているかどうか
bool controller::GetPush(button::button_type x, CONTROLLER_NUM num)
{
	return GamePadManager::GetState(num, button_key_code[x]) != 0;
}

// コントローラの状態取得
controller::button::button_state controller::GetButtonState(controller::button::button_type x, CONTROLLER_NUM num)
{
    switch (KEY(button_key_code[x], num))
    {
    case 0: return controller::button::button_state::bs_up;
    case 3: return controller::button::button_state::bs_trg;

    default:
        return controller::button::button_state::bs_down;
    }

    return controller::button::button_state::bs_up;
}

// スティックの傾きの値を得る
Vector2 controller::GetStickValue(stick::stick_type x, CONTROLLER_NUM num)
{
	Vector2 ret(
		GamePadManager::GetState(num, stick_key_code[x][0])*0.001f,
		GamePadManager::GetState(num, stick_key_code[x][1])*-0.001f
		);
	
	//補正
	if (fabsf(ret.x)<0.3f)ret.x = 0;
	if (fabsf(ret.y)<0.3f)ret.y = 0;

	if (Vector2Length(ret) > 1)return Vector2Normalize(ret);

	return ret;
}


// 指定したコントローラーを振動させる
void controller::SetVibration(DWORD power, float second,  CONTROLLER_NUM num, int pattern)
{
    GamePadManager::Vibration(num, pattern, (DWORD)(second*(float)DI_SECONDS), power);
}



#endif