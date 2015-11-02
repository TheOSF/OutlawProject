//
//
//          InputDevice.cpp
//
//


//
//     インクルードファイル
//
#include "InputDevice.h"
#include "InputManager.h"
#include "InputEffect.h"
#include "UtilFunc.h"
#include "../source/utillity/String.h"


//
//     [ InputManager::Device ]
//     // 接続されているデバイスクラス
//

// コンストラクタ
InputDevice::InputDevice(
	LPDIRECTINPUTDEVICE8 pDiDevice, 
	LPCDIDEVICEINSTANCE pDiDeviceInstance
	) :
	m_pDiDevice(pDiDevice)
{
	CopyMemory(&m_DiDeviceInstance, pDiDeviceInstance, sizeof(DIDEVICEINSTANCE));
}


// デストラクタ
InputDevice::~InputDevice()
{
	SafeRelease(m_pDiDevice);
	DeleteVectorPtr(m_pInputEffectArray);
}


// 初期化
bool InputDevice::Initialize(HWND hwnd)
{
	org::String controllerName;
	controllerName << _T("ゲームコントローラ [ ") << m_DiDeviceInstance.tszInstanceName << _T(" ] ");

	// データフォーマット設定
	 HRESULT hr = m_pDiDevice->SetDataFormat(&c_dfDIJoystick2);
	if (hr != DI_OK)
	{
		SafeRelease(m_pDiDevice);
		OutputDebugString(controllerName << _T("のデータフォーマットの設定に失敗しました。\n"));
		return false;
	}

	// 強調レベルの設定
	hr = m_pDiDevice->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (hr != DI_OK)
	{
		SafeRelease(m_pDiDevice);
		OutputDebugString(controllerName << _T("の強調レベルの設定に失敗しました。\n"));
		return false;
	}

	//	自動センタリング無効
	DIPROPDWORD	dipdw = { 0 };
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAUTOCENTER_OFF;

	hr = m_pDiDevice->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
	if (hr != DI_OK)
	{
		SafeRelease(m_pDiDevice);
		OutputDebugString(controllerName << _T("の自動センタリングの設定に失敗しました。\n"));
		return false;
	}

	// 各軸の設定
	hr = m_pDiDevice->EnumObjects(
		InputDevice::EnumAndSettingAxesCallback,
		(void*)m_pDiDevice,
		DIDFT_AXIS
		);
	if (hr != DI_OK)
	{
		SafeRelease(m_pDiDevice);
		OutputDebugString(controllerName << _T("の各軸の設定に失敗しました。\n"));
		return false;
	}

	// 振動エフェクト列挙 & 作成
	hr = m_pDiDevice->EnumEffects(
		EnumAndCreateEffectsCallback,
		(void*)this,
		DIEFT_PERIODIC     // PRERIODIC のみを列挙する ※他の奴は使い方わからん(泣)
		);
	if (hr != DI_OK)
	{
		SafeRelease(m_pDiDevice);
		OutputDebugString(controllerName << _T("の振動エフェクト列挙 & 作成に失敗しました。\n"));
		return false;
	}

	return true;
}


// デバイスが利用可能かチェック
bool InputDevice::IsDeviceUseOK()
{
	if (m_pDiDevice != nullptr)
	{
		HRESULT hr = m_pDiDevice->Acquire();

		return (hr == DI_OK) || (hr == S_FALSE);
	}
	return false;
}


// 状態を取得
bool InputDevice::Update(DIJOYSTATE2& outDiJoyState)
{
	if (m_pDiDevice == nullptr)return false;

	// ポーリング
	HRESULT hr = m_pDiDevice->Poll();

	if (hr != DI_OK)
	{
		if (IsDeviceUseOK() == false)return false;
		m_pDiDevice->Poll();
	}

	// 取得
	hr = m_pDiDevice->GetDeviceState(sizeof(DIJOYSTATE2), &outDiJoyState);

	return (hr == DI_OK) ? true : false;
}


// エフェクトにアクセス
InputEffect* InputDevice::AccessEffect(size_t index)
{
	if (index >= m_pInputEffectArray.size())return nullptr;
	return m_pInputEffectArray.at(index);
}


// 軸列挙 & 設定用
BOOL FAR PASCAL InputDevice::EnumAndSettingAxesCallback(LPCDIDEVICEOBJECTINSTANCE pDiDeviceObjectInstance, LPVOID pContext)
{
	// 軸範囲を設定（-1000～1000）
	DIPROPRANGE diprg = { 0 };

	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = pDiDeviceObjectInstance->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = +1000;

	LPDIRECTINPUTDEVICE8 pDiDevice = (LPDIRECTINPUTDEVICE8)pContext;

	HRESULT hr = pDiDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	return (hr == DI_OK) ? (DIENUM_CONTINUE) : DIENUM_STOP;
}


// 振動エフェクト列挙 & 作成用
BOOL FAR PASCAL InputDevice::EnumAndCreateEffectsCallback(LPCDIEFFECTINFO pDiEffectInfo, LPVOID pContext)
{
	InputDevice* pThis = (InputDevice*)pContext;

	pThis->CreateEffect(pDiEffectInfo);

	return DIENUM_CONTINUE;
}



// エフェクト作成
bool InputDevice::CreateEffect(LPCDIEFFECTINFO pDiEffectInfo)
{
	org::String effectName;
	effectName << _T("コントローラの振動エフェクト [ ") << pDiEffectInfo->tszName << " ] ";

	DIEFFECT diEffect = { 0 };
	LPDIRECTINPUTEFFECT pDiEffect = nullptr;

	DIPERIODIC diPeriodic = { 0 };
	diPeriodic.dwMagnitude = DI_FFNOMINALMAX;
	diPeriodic.lOffset = 0;
	diPeriodic.dwPhase = 0;
	diPeriodic.dwPeriod = (DWORD)(0.5f * DI_SECONDS);

	diEffect.cbTypeSpecificParams = sizeof(diPeriodic);
	diEffect.lpvTypeSpecificParams = &diPeriodic;

	// 振動エフェクト設定
	diEffect.dwSize = sizeof(diEffect);
	diEffect.dwFlags = DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration = 0;
	diEffect.dwSamplePeriod = 0;
	diEffect.dwGain = DI_FFNOMINALMAX;

	//	トリガー設定
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;
	diEffect.dwTriggerRepeatInterval = 0;

	//	エフェクト軸設定
	diEffect.dwFlags |= DIEFF_POLAR;
	DWORD Axes[] = { DIJOFS_X, DIJOFS_Y }; // エフェクト軸
	LONG Direction[] = { 1, 0 };				 // エフェクト方向
	diEffect.cAxes = 2;
	diEffect.rgdwAxes = Axes;
	diEffect.rglDirection = Direction;

	// エンベロープ構造体
	diEffect.lpEnvelope = NULL;

	// 作成
	HRESULT hr = m_pDiDevice->CreateEffect(
		pDiEffectInfo->guid,
		&diEffect,
		&pDiEffect,
		NULL
		);
	if (hr != DI_OK)
	{
		OutputDebugString(effectName << _T("の作成に失敗しました。\n"));
		return false;
	}

	// エフェクトクラス作成
	InputEffect* pEffect = new InputEffect(pDiEffect, pDiEffectInfo);

	// エフェクト追加
	m_pInputEffectArray.push_back(
		pEffect
		);

	return true;
}
