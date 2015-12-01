#pragma once

#include "SoccerAttackClass.h"



//****************************************************************
//		サッカーの攻撃情報クラス
//****************************************************************

class SoccerAttackInfo_UsualAtk :public SoccerAttackClass::AttackInfo
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
		float              AddSkillValue;
	};

	Param m_Param;

	SoccerAttackInfo_UsualAtk(
		SoccerPlayer* pOwner
		);

	~SoccerAttackInfo_UsualAtk();

private:

	SoccerPlayer * const  m_pOwner;
	Vector3               m_LocusPos, m_LocusVec;

	//ダメージパラメータをセット
	void DamageParamSet(DamageShpere* pDmg);

	//ダメージ位置をセット
	void DamagePosSet(DamageShpere* pDmg, SoccerPlayer* pTennis);

	//モーションをセット
	void MotionSet(SoccerPlayer* pTennis);

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
	void Update(int Frame, Locus* pLocus);

	//攻撃が当たった時に呼ばれる
	void HitAttack(DamageShpere* pDmg);
};