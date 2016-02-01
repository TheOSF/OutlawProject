//
//
//          GamePad.cpp
//
//


//
//     インクルードファイル
//
#include "GamePad.h"
#include "../InputDevice.h"
#include "../InputEffect.h"



//
//     [ GamePad ]
//     ゲームパッドクラス
//

// デフォルト設定
const int GamePad::DefaultMapping[GamePadIndex_Max] = 
{
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
};

// ELECOM
GamePadSet GamePad::ELECOM = { 
	0,1,2,5,
	3,4,
	6,7,8,
	9,10,11,12,

	13,15,16,14,
	19,17,
	20,18,
	23,24,
	21,22
};


// コンストラクタ
GamePad::GamePad(InputDevice* pInputDevice) :
	m_pInputDevice(pInputDevice)
{
	ZeroMemory(m_State, sizeof(int) * GamePadIndex_Max);

	CopyMemory(m_Mapping, DefaultMapping, sizeof(int) * GamePadIndex_Max);

	Asign(ELECOM);
}


// デストラクタ
GamePad::~GamePad()
{
	
}


// 更新
bool GamePad::Update()
{
	if (m_pInputDevice == nullptr)return false;

    ZeroMemory(m_CallCounter, sizeof(int)*GamePadIndex_Max * 4);

	DIJOYSTATE2 JoyState = { 0 };
	if (m_pInputDevice->Update(JoyState) == false)return false;

	//	軸状態保存
	long axis[9] = { 0 };
	axis[GamePadIndex_LeftStick_X] = JoyState.lX;
	axis[GamePadIndex_LeftStick_Y] = JoyState.lY;
	axis[GamePadIndex_RightStick_X] = JoyState.lZ;
	axis[GamePadIndex_RightStick_Y] = JoyState.lRx;
	axis[GamePadIndex_LeftTrigger] = JoyState.lRy;
	axis[GamePadIndex_RightTrigger] = JoyState.lRz;
	axis[GamePadIndex_Acce_X] = JoyState.rglSlider[0];
	axis[GamePadIndex_Acce_Y] = JoyState.rglSlider[1];

	//	軸設定
	m_State[GamePadIndex_LeftStick_X] = axis[m_Mapping[GamePadIndex_LeftStick_X]];
	m_State[GamePadIndex_LeftStick_Y] = axis[m_Mapping[GamePadIndex_LeftStick_Y]];
	m_State[GamePadIndex_RightStick_X] = axis[m_Mapping[GamePadIndex_RightStick_X]];
	m_State[GamePadIndex_RightStick_Y] = axis[m_Mapping[GamePadIndex_RightStick_Y]];
	m_State[GamePadIndex_LeftTrigger] = axis[m_Mapping[GamePadIndex_LeftTrigger]];
	m_State[GamePadIndex_RightTrigger] = axis[m_Mapping[GamePadIndex_RightTrigger]];
	m_State[GamePadIndex_Acce_X] = axis[m_Mapping[GamePadIndex_Acce_X]];
	m_State[GamePadIndex_Acce_Y] = axis[m_Mapping[GamePadIndex_Acce_Y]];
	m_State[GamePadIndex_Acce_Z] = axis[m_Mapping[GamePadIndex_Acce_Z]];


	// ハットスイッチ更新
	// 角度を方向番号に変換
	int Direction = (JoyState.rgdwPOV[0] + 2200) % 36000 / 4500;
	const u8 POV[] = { 0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x00 };
	if (LOWORD(JoyState.rgdwPOV[0]) == 0xFFFF)
	{
		// 何も入力されていない状態
		Direction = 8;
	}
	for (int i = GamePadIndex_LeftPad_Up; i < GamePadIndex_RightPad_Up; i++)
	{
		if (POV[Direction] & (0x01 << (i- GamePadIndex_LeftPad_Up)))
		{
			if (m_State[i] & 0x01)m_State[i] = 1; else m_State[i] = 3;
		}
		else
		{
			if (m_State[i] & 0x01)m_State[i] = 2; else m_State[i] = 0;
		}
	}


	// ボタン更新
	for (int i = GamePadIndex_RightPad_Up; i < GamePadIndex_Max; i++)
	{
		if (JoyState.rgbButtons[m_Mapping[i] - GamePadIndex_RightPad_Up] & 0x80)
		{
			if (m_State[i] & 0x01)m_State[i] = 1; else m_State[i] = 3;
		}
		else
		{
			if (m_State[i] & 0x01)m_State[i] = 2; else m_State[i] = 0;
		}
	}

	return true;
}


// 利用可能かどうか
bool GamePad::IsAvailable()const
{
	return m_pInputDevice->IsDeviceUseOK();
}


// 状態取得
int GamePad::GetState(GamePadIndex index, int* pCallCount)
{
    if ( m_State[index] >= 0 && m_State[index] < 4 ) 
    {
        m_CallCounter[index][m_State[index]] += 1;
        if ( pCallCount ) {
            *pCallCount = m_CallCounter[index][m_State[index]];
        }
    }
	return m_State[index];
}


// マッピング設定
bool GamePad::Asign(const GamePadSet& set)
{
	m_Mapping[GamePadIndex_LeftStick_X] = set.LeftStick_X;
	m_Mapping[GamePadIndex_LeftStick_Y] = set.LeftStick_Y;

	m_Mapping[GamePadIndex_RightStick_X] = set.RightStick_X;
	m_Mapping[GamePadIndex_RightStick_Y] = set.RightStick_Y;

	m_Mapping[GamePadIndex_LeftTrigger] = set.LeftTrigger;
	m_Mapping[GamePadIndex_RightTrigger] = set.RightTrigger;

	m_Mapping[GamePadIndex_Acce_X] = set.Acce_X;
	m_Mapping[GamePadIndex_Acce_Y] = set.Acce_Y;
	m_Mapping[GamePadIndex_Acce_Z] = set.Acce_Z;

	m_Mapping[GamePadIndex_LeftPad_Up] = set.LeftPad_Up;
	m_Mapping[GamePadIndex_LeftPad_Down] = set.LeftPad_Down;
	m_Mapping[GamePadIndex_LeftPad_Left] = set.LeftPad_Left;
	m_Mapping[GamePadIndex_LeftPad_Right] = set.LeftPad_Right;

	m_Mapping[GamePadIndex_RightPad_Up] = set.RightPad_Up;
	m_Mapping[GamePadIndex_RightPad_Down] = set.RightPad_Down;
	m_Mapping[GamePadIndex_RightPad_Left] = set.RightPad_Left;
	m_Mapping[GamePadIndex_RightPad_Right] = set.RightPad_Right;

	m_Mapping[GamePadIndex_LeftShoulder1] = set.LeftShoulder1;
	m_Mapping[GamePadIndex_LeftShoulder2] = set.LeftShoulder2;

	m_Mapping[GamePadIndex_RightShoulder1] = set.RightShoulder1;
	m_Mapping[GamePadIndex_RightShoulder2] = set.RightShoulder2;

	m_Mapping[GamePadIndex_LeftStick_Button] = set.LeftStick_Button;
	m_Mapping[GamePadIndex_RightStick_Button] = set.RightStick_Button;

	m_Mapping[GamePadIndex_Start] = set.Start;
	m_Mapping[GamePadIndex_Back_Select] = set.Back_Select;

	return true;
}


// 振動させる
bool GamePad::Vibration(size_t index, DWORD period, DWORD gain)
{
	if (m_pInputDevice == nullptr)return false;

	InputEffect* pInputEffect = m_pInputDevice->AccessEffect(index);

	if (pInputEffect == nullptr)return false;
	
	return pInputEffect->Vibration(period, gain);
}

