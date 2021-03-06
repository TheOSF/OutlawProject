#pragma once

#include "BaseballAttackClass.h"



//****************************************************************
//		野球の攻撃情報クラス
//****************************************************************

class BaseballAttackInfo_UsualAtk :public BaseballAttackClass::AttackInfo
{
public:
	struct Param
	{
		float              DamageValue;               //ダメージ値
		float              DamageSize;                //当たり判定大きさ
		float              DamagePosLength;           //ボーンから当たり判定までの距離
		DamageBase::Type   DamageType;                //ダメージタイプ

		int                DamageEnableStart;         //ダメージ有効スタートフレーム
		int                DamageEnableEnd;           //ダメージ有効エンドフレーム

		float              MoveSpeed;
		int                MoveStartFrame;
		int                MoveEndFrame;

		int                Motion;

		int                AllFrame;

		int                CanComboButtonPushFrame;

		int                ComboSwitchStartFrame;
		int                ComboSwitchEndFrame;

		int                AngleControllStartFrame;
		int                AngleControllEndFrame;
		RADIAN             AngleControllValue;

		int                LocusBoneNum;

		Vector2            VanishValue;

		bool			   changeFlg;

	};

	Param m_Param;

	BaseballAttackInfo_UsualAtk(
		BaseballPlayer* pOwner
		);

	~BaseballAttackInfo_UsualAtk();

private:

	BaseballPlayer * const  m_pOwner;
	Vector3               m_LocusPos, m_LocusVec;
	bool				  VanishFlg;
	//ダメージパラメータをセット
	void DamageParamSet(DamageShpere* pDmg);

	//ダメージ位置をセット
	void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pBaseball);

	//モーションをセット
	void MotionSet(BaseballPlayer* pTennis);

	//引数のフレームが攻撃終了かどうか
	bool isEnd(int Frame);

	//引数のフレームがコンボ移行フレームかどうか
	bool isComboSwitchFrame(int Frame);

	//引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
	bool isComboButtonFrame(int Frame);

	//引数のフレームが、角度調整を行えるフレームかどうか
	bool isCanAngleControll(int Frame, RADIAN& OutControllVal);

	//引数のフレームが、ダメージ判定有効フレームかどうか
	bool isDamageEnable(int Frame);

	//毎フレーム呼ばれる
	void Update(int Frame, LocusHDR* pLocus);

	//攻撃があたったときに呼ばれる
	void HitAttack(DamageShpere* pDmg);
    
    //カウンターヒットかどうか
    bool isCounterHitFrame(int Frame);
};