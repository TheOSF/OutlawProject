#ifndef __BASEBALL_EVASION_H__
#define __BASEBALL_EVASION_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"


//プレイヤー操作の回避クラス
class CharacterEvasion;
class BaseballState_PlayerControll_Evasion :public BaseballState{
private:
	float roolspeed;//　回避時の速さ
private:
	// 回避クラス作成
	CharacterEvasion* CreateEvasionClass(BaseballPlayer* b);
private:
	// 回避クラス
	CharacterEvasion* m_pEvasionClass;
public:
	//　コンストラクタ
	BaseballState_PlayerControll_Evasion(float speed);
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;

};

#endif