//
//
//          GamePadManager.cpp
//
//


//
//     インクルードファイル
//
#include "GamePadManager.h"
#include "../InputManager.h"
#include "../UtilFunc.h"


//
//     [ GamePadManager ]
//     ゲームパッド管理クラス
//

// ゲームパッド配列
std::vector<GamePad*> GamePadManager::pGamePadArray;


// 初期化
bool GamePadManager::Initialize(HWND hwnd)
{
	if (InputManager::Initialize() == false)return false;

	// デバイス作成
	std::vector<InputDevice*> pInputDeviceArray =
		InputManager::EnumAndCreateDevices(hwnd);

	// ゲームパッド作成
	for (auto& it : pInputDeviceArray)
	{
		pGamePadArray.push_back(new GamePad(it));
	}
	return true;
}


// 解放
bool GamePadManager::Release()
{
	DeleteVectorPtr(pGamePadArray);

	return InputManager::Release();
}


// 更新
std::vector<size_t> GamePadManager::Update()
{
	std::vector<size_t> result;

	size_t count = 0;
	for (auto& it : pGamePadArray)
	{
		if (it->Update() == false)
		{
			result.push_back(count);
		}
		count++;
	}

	return result;
}


// 利用可能かどうか
bool GamePadManager::IsAvailable(size_t n)
{
	if (n >= pGamePadArray.size())return false;

	return pGamePadArray.at(n)->IsAvailable();
}


// 状態取得
int GamePadManager::GetState(size_t n, GamePadIndex index, int* pCallCount)
{
	if (n >= pGamePadArray.size())return 0;

	return pGamePadArray.at(n)->GetState(index, pCallCount);
}


// マッピング設定
bool GamePadManager::Asign(size_t n, const GamePadSet& set)
{
	if (n >= pGamePadArray.size())return false;

	return pGamePadArray.at(n)->Asign(set);
}


// マッピング設定
std::vector<size_t> GamePadManager::Asign(std::vector<GamePadSet> & set)
{
	std::vector<size_t> result;

	size_t count = 0;
	for (auto& it : set)
	{
		if (count >= pGamePadArray.size())break;

		if (pGamePadArray.at(count)->Asign(it) == false)
		{
			result.push_back(count);
		}
		count++;
	}

	return result;
}


// 振動させる
bool GamePadManager::Vibration(size_t n, size_t effect, DWORD period, DWORD gain)
{
	if (n >= pGamePadArray.size())return false;

	return pGamePadArray.at(n)->Vibration(effect, period, gain);
}
