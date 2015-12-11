#pragma once

#include "BaseballPlayer.h"
#include "../../Render/LightObject.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	BaseballState_SPAttack_B(BaseballPlayer* b);
	~BaseballState_SPAttack_B();

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	void(BaseballState_SPAttack_B::*m_pStateFunc)();

	int                     m_Timer;
	bool					timeflg;
	BaseballPlayer*         m_pBaseBall;
	DamageShpere            m_Damage;
	UINT                    m_ChrLiveCount;
	PointLight              m_Light;

	
	//void State_Atk1();			 //　初撃
	//void State_Atk1End();		 //　初撃外れた
	void State_Atk2();			 //　2撃目

	void State_Finish()
	{
		//　雷エフェクト発動
		ThunderInvoke(8);
	}		 //　必殺終わり

	void FreezeGame(UINT frame); //　The World

	void ThunderInvoke(UINT point_num);
};

