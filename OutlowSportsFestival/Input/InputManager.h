//
//
//          InputManager.h
//
//


#pragma once


//
//     インクルードファイル
//
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <vector>


//
//     前方宣言
//
class InputDevice;


//
//     [ InputManager ]
//     入力管理クラス
//
class InputManager
{
public:

	// 初期化
	static bool Initialize();


	// 解放
	static bool Release();


	// デバイスを 列挙 & 作成 する
	static std::vector<InputDevice*> EnumAndCreateDevices(HWND hwnd);


private:
	// デバイス列挙 & 作成用
	static BOOL FAR PASCAL EnumAndCreateDevicesCallback(LPCDIDEVICEINSTANCE pDiDeviceInstance, LPVOID pContext);


	// デバイス作成関数
	static bool CreateDevice(LPCDIDEVICEINSTANCE pDiDeviceInstance, LPVOID pContext);


	InputManager() = delete;
	~InputManager() = delete;
	InputManager(const InputManager&) = delete;
	void operator = (const InputManager&) = delete;

private:
	static bool IsInitialized; // 初期化フラグ

	static LPDIRECTINPUT8 pDirectInput; // DirectInput8 オブジェクト

	static std::vector < InputDevice* > pDeviceArray; // 接続されているデバイス配列	

};

