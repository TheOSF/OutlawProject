#pragma once

#include "BaseballSpAttackClass.h"



//****************************************************************
//		野球の攻撃情報クラス
//****************************************************************

class BaseballAttackInfo_SpAtk :public BaseballSpAttackClass::AttackInfo
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



		int                AngleControllStartFrame;
		int                AngleControllEndFrame;
		RADIAN             AngleControllValue;

		int                LocusBoneNum;
	};

	Param m_Param;

	BaseballAttackInfo_SpAtk(BaseballPlayer* pOwner);
	~BaseballAttackInfo_SpAtk();

private:

	BaseballPlayer * const  m_pOwner;
	Vector3               m_LocusPos, m_LocusVec;

	//ダメージパラメータをセット
	void DamageParamSet(DamageShpere* pDmg);

	//ダメージ位置をセット
	void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pTennis);

	//モーションをセット
	void MotionSet(BaseballPlayer* pTennis);

	//引数のフレームが攻撃終了かどうか
	bool isEnd(int Frame);

	
	//引数のフレームが、角度調整を行えるフレームかどうか
	bool isCanAngleControll(int Frame, RADIAN& OutControllVal);

	//引数のフレームが、ダメージ判定有効フレームかどうか
	bool isDamageEnable(int Frame);

	//毎フレーム呼ばれる
	void Update(int Frame, LocusHDR* pLocus);
};