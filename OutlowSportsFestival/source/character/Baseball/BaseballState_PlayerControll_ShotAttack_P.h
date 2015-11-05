#ifndef __BASEBALL_SHOTATTACK_P_H__
#define __BASEBALL_SHOTATTACK_P_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"



//プレイヤー操作の遠距離クラス(ピッチャー)
class CharacterShotAttack;
class BaseballState_PlayerControll_ShotAttack_P :public BaseballState{
private:
	//　遠距離クラス作成(ピッチャー)
	CharacterShotAttack* CreateShotAttackClass_P(BaseballPlayer* b);
private:
	//　遠距離クラス
	CharacterShotAttack* m_pShotAttackClass_P;
public:

	//　コンストラクタ
	BaseballState_PlayerControll_ShotAttack_P();
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;

public:
	void ComExcute(BaseballPlayer* b);
};

#endif