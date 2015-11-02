//
//
//          GamePadSet.h
//
//


#pragma once


//
//     インクルードファイル
//


//
//     [ GamePadSet ]
//     マッピング設定用構造体
//
struct GamePadSet
{
	u8 LeftStick_X;
	u8 LeftStick_Y;

	u8 RightStick_X;
	u8 RightStick_Y;

	u8 LeftTrigger;
	u8 RightTrigger;

	u8 Acce_X;
	u8 Acce_Y;
	u8 Acce_Z;

	u8 LeftPad_Up;
	u8 LeftPad_Down;
	u8 LeftPad_Left;
	u8 LeftPad_Right;

	u8 RightPad_Up;
	u8 RightPad_Down;
	u8 RightPad_Left;
	u8 RightPad_Right;

	u8 LeftShoulder1;
	u8 LeftShoulder2;

	u8 RightShoulder1;
	u8 RightShoulder2;

	u8 LeftStick_Button;
	u8 RightStick_Button;

	u8 Start;
	u8 Back_Select;

};



