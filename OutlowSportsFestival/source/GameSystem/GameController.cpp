#include "GameController.h"
#include "IEX_Input.h"


//スティックを倒したかどうかの記録(４つまで)
static bool g_SaveStickValues[4][2] = 
{
    false
};

static const float g_AdjustStickValue = 0.3f;


//引数の値がスティックの誤差の値以内かどうか
bool controller::isStickAdjustValue(float value)
{
    return fabsf(value) < g_AdjustStickValue;
}


//スティック(またはカーソル)を倒した瞬間かどうかを得る
bool controller::GetStickJustMove(stick::stick_type x, CONTROLLER_NUM num)
{
    return
        g_SaveStickValues[num][x] == false &&
        Vector2LengthSq(GetStickValue(x, num)) > g_AdjustStickValue;
}


//更新(全体で１フレームに一回呼び出すだけ、使わないで！)
void controller::UpdateInfo()
{
    for ( int i = 0; i < 4; ++i )
    {
        for ( int j = 0; j < 2; ++j )
        {
            g_SaveStickValues[i][j] = Vector2LengthSq(GetStickValue((stick::stick_type)j, i)) > g_AdjustStickValue;
        }
    }
}



#ifdef OUTLAW2_CONTROLL_TYPE_IEX_INPUT



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
bool controller::GetTRG(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    return KEY_Get(button_key_code[x], num, pCallCount) == 3;
}

bool controller::GetLeave(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    return KEY_Get(button_key_code[x], num, pCallCount) == 2;
}

//押しているかどうか
bool controller::GetPush(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    return KEY_Get(button_key_code[x], num, pCallCount) != 0;
}

//押しているかどうか(誰かが)
bool controller::GetPushAnyController(button::button_type x)
{
    for(int i = 0 ; i < 4 ; ++i)
    {
        if ( KEY_Get(button_key_code[x], i, nullptr) != 0 )
        {
            return true;
        }
    }
    return false;
}

//押した瞬間かどうか(誰かが)
bool	controller::GetTRGAnyController(button::button_type x)
{
    for ( int i = 0; i < 4; ++i ) {
        if ( KEY_Get(button_key_code[x], i, nullptr) == 3 ) {
            return true;
        }
    }
    return false;
}

// コントローラの状態取得
controller::button::button_state controller::GetButtonState(controller::button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    switch ( KEY_Get(button_key_code[x], num, pCallCount))
    {
    case 0: return controller::button::button_state::bs_up;
    case 3: return controller::button::button_state::bs_trg;

    default:
        return controller::button::button_state::bs_down;
    }

    return controller::button::button_state::bs_up;
}

//方向キーの方向を得る
static Vector2 GetCursorValue(controller::CONTROLLER_NUM num)
{
    Vector2 ret(0, 0);

    ret.x += (controller::GetPush(controller::button::right,num)) ? (1) : (0);
    ret.x += (controller::GetPush(controller::button::left, num)) ? (-1) : (0);

    ret.y += (controller::GetPush(controller::button::up, num))  ? (1) : (0);
    ret.y += (controller::GetPush(controller::button::down, num))  ? (-1) : (0);

    return ret;
}

//スティックの傾きの値を得る
Vector2 controller::GetStickValue(stick::stick_type x, CONTROLLER_NUM num)
{
    Vector2 ret(KEY_Get(stick_key_code[x][0], num, nullptr)*0.001f, KEY_Get(stick_key_code[x][1], num, nullptr)*-0.001f);

    //補正
    if (fabsf(ret.x)<g_AdjustStickValue)ret.x = 0;
    if (fabsf(ret.y)<g_AdjustStickValue)ret.y = 0;

    //左スティックならカーソルも考慮する
    if (x == stick::left)
    {
        Vector2 cur = GetCursorValue(num);

        if (fabsf(ret.x) < fabsf(cur.x))
        {
            ret.x = cur.x;
        }

        if (fabsf(ret.y) < fabsf(cur.y))
        {
            ret.y = cur.y;
        }
    }

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



//方向キーの方向を得る
static Vector2 GetCursorValue(controller::CONTROLLER_NUM num)
{
    Vector2 ret(0, 0);

    ret.x += (GamePadManager::GetState(num, button_key_code[controller::button::right]) > 0) ? (1) : (0);
    ret.x += (GamePadManager::GetState(num, button_key_code[controller::button::left]) > 0) ? (-1) : (0);

    ret.y += (GamePadManager::GetState(num, button_key_code[controller::button::up]) > 0) ? (1) : (0);
    ret.y += (GamePadManager::GetState(num, button_key_code[controller::button::down]) > 0) ? (-1) : (0);

    return ret;
}


// 押した瞬間かどうか
bool controller::GetTRG(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
	return GamePadManager::GetState(num, button_key_code[x],pCallCount) == 3;
}

bool controller::GetLeave(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    return GamePadManager::GetState(num, button_key_code[x], pCallCount) == 2;
}

// 押しているかどうか
bool controller::GetPush(button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    return GamePadManager::GetState(num, button_key_code[x], pCallCount) != 0;
}

//押しているかどうか(誰かが)
bool controller::GetPushAnyController(button::button_type x)
{
    for (int i = 0; i < 4; ++i)
    {
        if (GamePadManager::GetState((size_t)i, button_key_code[x]) != 0)
        {
            return true;
        }
    }
    return false;
}

//押した瞬間かどうか(誰かが)
bool	controller::GetTRGAnyController(button::button_type x) {
    for ( int i = 0; i < 4; ++i ) {
        if ( GamePadManager::GetState((size_t)i, button_key_code[x]) == 3 ) {
            return true;
        }
    }
    return false;
}

// コントローラの状態取得
controller::button::button_state controller::GetButtonState(controller::button::button_type x, CONTROLLER_NUM num, int* pCallCount)
{
    switch (GamePadManager::GetState((size_t)num, button_key_code[x], pCallCount))
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

    //左スティックならカーソルも考慮する
    if (x == stick::left)
    {
        Vector2 cur = GetCursorValue(num);

        if (fabsf(ret.x) < fabsf(cur.x))
        {
            ret.x = cur.x;
        }

        if (fabsf(ret.y) < fabsf(cur.y))
        {
            ret.y = cur.y;
        }
    }
	
	//補正
    if (fabsf(ret.x)<g_AdjustStickValue)ret.x = 0;
    if (fabsf(ret.y)<g_AdjustStickValue)ret.y = 0;

	if (Vector2Length(ret) > 1)return Vector2Normalize(ret);

	return ret;
}



// 指定したコントローラーを振動させる
void controller::SetVibration(DWORD power, float second,  CONTROLLER_NUM num, int pattern)
{
    GamePadManager::Vibration(num, pattern, (DWORD)(second*(float)DI_SECONDS), power);
}


#endif