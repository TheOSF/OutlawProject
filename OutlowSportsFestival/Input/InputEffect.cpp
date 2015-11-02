//
//
//          InputEffect.cpp
//
//


//
//     インクルードファイル
//
#include "InputEffect.h"
#include "InputDevice.h"
#include "UtilFunc.h"
#include "InputError.h"
#include "../source/utillity/String.h"


//
//     [ InputEffect ]
//

// コンストラクタ
InputEffect::InputEffect(
	LPDIRECTINPUTEFFECT pDiEffect, 
	LPCDIEFFECTINFO pDiEffectInfo
	) :
	m_pDiEffect(pDiEffect)
{
	CopyMemory(&m_DiEffectInfo, pDiEffectInfo, sizeof(DIEFFECTINFO));
}


// デストラクタ
InputEffect::~InputEffect()
{
	if (m_pDiEffect != nullptr)
	{
		m_pDiEffect->Unload();
		m_pDiEffect->Release();
		m_pDiEffect = nullptr;
	}
}


// エフェクトスタート
bool InputEffect::StartEffect()
{
	if (m_pDiEffect == nullptr)return false;

	HRESULT hr = m_pDiEffect->Start(1, 0);

	return (hr == DI_OK) ? true : false;
}


// エフェクトストップ
bool InputEffect::StopEffect()
{
	if (m_pDiEffect == nullptr)return false;

	HRESULT hr = m_pDiEffect->Stop();

	return (hr == DI_OK) ? true : false;
}


// 設定
bool InputEffect::SetParameters(DWORD period, DWORD gain)
{
	if (m_pDiEffect == nullptr)return false;

	DIEFFECT diEffect = { 0 };

	// 振動エフェクト設定
	diEffect.dwSize = sizeof(diEffect);
	diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	diEffect.dwDuration = period; // エフェクト継続時間
	diEffect.dwGain = gain;

	HRESULT hr = m_pDiEffect->SetParameters(&diEffect, DIEP_DURATION | DIEP_GAIN);

	return (SUCCEEDED(hr)) ? true : false;
}


// 再生
bool InputEffect::Vibration(DWORD period, DWORD gain)
{
	StopEffect();
	if (SetParameters(period, gain) == false)return false;
	return StartEffect();
}

