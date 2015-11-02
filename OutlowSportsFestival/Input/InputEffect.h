//
//
//          InputEffect.h
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


//
//     [ InputEffect ]
//
class InputEffect
{
public:
	// コンストラクタ
	InputEffect(LPDIRECTINPUTEFFECT pDiEffect, LPCDIEFFECTINFO pDiEffectInfo);


	// デストラクタ
	~InputEffect();


	// エフェクトスタート
	bool StartEffect();


	// エフェクトストップ
	bool StopEffect();


	// 設定
	//bool SetParameters(LONG Start, LONG End);


	// 設定
	bool SetParameters(DWORD period, DWORD gain);


	// 再生
	bool Vibration(DWORD period, DWORD gain);

private:

	LPDIRECTINPUTEFFECT m_pDiEffect; // エフェクト
	DIEFFECTINFO m_DiEffectInfo; // エフェクト情報

};

