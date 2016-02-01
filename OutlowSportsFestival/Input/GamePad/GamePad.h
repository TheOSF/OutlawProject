//
//
//          GamePad.h
//
//


#pragma once


//
//     インクルードファイル
//
#include "GamePadSet.h"
#include "GamePadIndex.h"


//
//     前方宣言
//
class InputDevice;


//
//     [ GamePad ]
//     ゲームパッドクラス
//
class GamePad
{
public:
	// コンストラクタ
	GamePad(InputDevice* pInputDevice);


	// デストラクタ
	~GamePad();


	// 更新
	bool Update();
	

	// 利用可能かどうか
	bool IsAvailable()const;


	// 状態取得
    int GetState(GamePadIndex index, int* pCallCount = nullptr);


	// マッピング設定
	bool Asign(const GamePadSet& set);


	// 振動させる
	bool Vibration(size_t index, DWORD period, DWORD gain);

private:
	InputDevice* m_pInputDevice; // デバイス
	int m_Mapping[GamePadIndex_Max];  // マッピング用
	int m_State[GamePadIndex_Max]; // 状態
    int m_CallCounter[GamePadIndex_Max][4]; // 呼び出しカウンタ

	// デフォルト設定
	static const int DefaultMapping[GamePadIndex_Max];
	static GamePadSet ELECOM;
};

