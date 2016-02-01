//
//
//          GamePadManager.h
//
//


#pragma once


//
//     インクルードファイル
//
#include "GamePad.h"
#include <vector>



//
//     [ GamePadManager ]
//     ゲームパッド管理クラス
//
class GamePadManager
{
public:

	// 初期化
	static bool Initialize(HWND hwnd);


	// 解放
	static bool Release();


	// 更新
	static std::vector<size_t> Update();


	// 利用可能かどうか
	static bool IsAvailable(size_t n);


	// 状態取得
    static int GetState(size_t n, GamePadIndex index, int* pCallCount = nullptr);


	// マッピング設定
	static bool Asign(size_t n, const GamePadSet& set);


	// マッピング設定
	static std::vector<size_t> Asign(std::vector<GamePadSet> & set);


	// 振動させる
	static bool Vibration(size_t n, size_t effect, DWORD period, DWORD gain);

	GamePadManager() = delete;
	~GamePadManager() = delete;
	GamePadManager(const GamePadManager&) = delete;
	void operator = (const GamePadManager&) = delete;

private:
	static std::vector<GamePad*> pGamePadArray; // ゲームパッド配列
};

