//
//
//          InputManager.cpp
//
//


//
//     インクルードファイル
//
#include "InputManager.h"
#include "InputDevice.h"
#include "UtilFunc.h"
#include "../source/utillity/String.h"



//
//     [ InputManager ]
//     入力管理クラス
//

// 初期化フラグ
bool InputManager::IsInitialized = false;

// DirectInput8 オブジェクト
LPDIRECTINPUT8 InputManager::pDirectInput = nullptr;

// 接続されているデバイス配列	
std::vector<InputDevice*> InputManager::pDeviceArray;


// 初期化
bool InputManager::Initialize()
{
	if (IsInitialized == true)return false;

	// DirectInput8 オブジェクトの作成
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&pDirectInput,
		NULL
		);
	if (hr != DI_OK) 
	{
		OutputDebugString(_T("DirectInput8 オブジェクトの作成に失敗しました。\n"));
		return false;
	}

	// DirectInput8 オブジェクトの初期化
	hr = pDirectInput->Initialize(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION
		);
	if (hr != DI_OK)
	{
		OutputDebugString(_T("DirectInput8 オブジェクトの初期化に失敗しました。\n"));
		SafeRelease(pDirectInput);
		return false;
	}

	// 初期化フラグ設定
	IsInitialized = true;

	return true;
}


// 解放
bool InputManager::Release()
{
	if (IsInitialized == false)return false;

	// デバイス情報解放
	DeleteVectorPtr(pDeviceArray);

	// DirectInput8 オブジェクト解放
	SafeRelease(pDirectInput);

	// 初期化フラグ設定
	IsInitialized = false;
	
	return true;
}


// デバイスを 列挙 & 作成 する
std::vector<InputDevice*> InputManager::EnumAndCreateDevices(HWND hwnd)
{
	// ゲームコントローラを列挙
	HRESULT hr = pDirectInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		InputManager::EnumAndCreateDevicesCallback,
		(void*)hwnd,
		DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK
		);

	if (hr != DI_OK)
	{
		OutputDebugString(_T("ゲームコントローラの列挙が正常に終了しませんでした。\n"));
	}

	return pDeviceArray;
}


// デバイス列挙 & 作成用
BOOL FAR PASCAL InputManager::EnumAndCreateDevicesCallback(LPCDIDEVICEINSTANCE pDiDeviceInstance, LPVOID pContext)
{
	bool result = CreateDevice(pDiDeviceInstance, pContext);
	
	return result ? DIENUM_CONTINUE : DIENUM_STOP;
}


// デバイス作成関数
bool InputManager::CreateDevice(LPCDIDEVICEINSTANCE pDiDeviceInstance, LPVOID pContext)
{
	if (IsInitialized == false)return false;
	if (pDirectInput == nullptr)return false;

	LPDIRECTINPUTDEVICE8 pDiDevice = nullptr;

	org::String controllerName;
	controllerName << _T("ゲームコントローラ [ ") << pDiDeviceInstance->tszInstanceName << _T(" ] ");

	HRESULT hr = pDirectInput->CreateDevice(
		pDiDeviceInstance->guidInstance,
		&pDiDevice,
		NULL
		);
	if (hr != DI_OK)
	{
		OutputDebugString(controllerName << _T(" の作成に失敗しました。\n"));
		return false;
	}

	// デバイスクラス作成
	InputDevice* pDevice = new InputDevice(
		pDiDevice,
		pDiDeviceInstance
		);

	// デバイス初期化
	if (pDevice->Initialize((HWND)pContext) == false)
	{
		delete pDevice;
		OutputDebugString(controllerName << _T(" の初期化に失敗しました。\n"));
		return false;
	}

	// デバイス追加
	pDeviceArray.push_back(pDevice);
	return true;
}


