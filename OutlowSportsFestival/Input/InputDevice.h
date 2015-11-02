//
//
//          InputDevice.h
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
class InputEffect;


//
//     [ InputDevice ]
//     // 接続されているデバイスクラス
//
class InputDevice
{
public:
	// コンストラクタ
	InputDevice(
		LPDIRECTINPUTDEVICE8 pDiDevice, 
		LPCDIDEVICEINSTANCE pDiDeviceInstance
		);


	// デストラクタ
	~InputDevice();


	// 初期化
	bool Initialize(HWND hwnd);


	// デバイスが利用可能かチェック
	bool IsDeviceUseOK();


	// 状態を取得
	bool Update(DIJOYSTATE2& outDiJoyState);


	// エフェクトにアクセス
	InputEffect* AccessEffect(size_t index);

private:
	// 軸列挙 & 設定用
	static BOOL FAR PASCAL EnumAndSettingAxesCallback(LPCDIDEVICEOBJECTINSTANCE pDiDeviceObjectInstance, LPVOID pContext);


	// 振動エフェクト列挙 & 作成用
	static BOOL FAR PASCAL EnumAndCreateEffectsCallback(LPCDIEFFECTINFO pDiEffectInfo, LPVOID pContext);


	// エフェクト作成
	bool CreateEffect(LPCDIEFFECTINFO pDiEffectInfo);


	// コピー禁止
	InputDevice(const InputDevice&) = delete;

	// コピー禁止
	void operator = (const InputDevice&) = delete;

private:
	DIDEVICEINSTANCE m_DiDeviceInstance; // 接続されているデバイスの情報
	LPDIRECTINPUTDEVICE8 m_pDiDevice; // 接続されているデバイス・オブジェクト
	std::vector<InputEffect*> m_pInputEffectArray; // エフェクト配列

};


