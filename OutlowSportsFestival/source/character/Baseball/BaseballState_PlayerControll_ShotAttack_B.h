#ifndef __BASEBALL_SHOTATTACK_B_H__
#define __BASEBALL_SHOTATTACK_B_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"




//プレイヤー操作の遠距離クラス(バッター)
class CharacterShotAttack;
class BaseballState_PlayerControll_ShotAttack_B :public BaseballState{
private:
	//　遠距離クラス作成(バッター)
	CharacterShotAttack* CreateShotAttackClass_B(BaseballPlayer* b);
private:
	//　遠距離クラス
	CharacterShotAttack* m_pShotAttackClass_B;
public:
	//　コンストラクタ
	BaseballState_PlayerControll_ShotAttack_B();
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;
};

#endif