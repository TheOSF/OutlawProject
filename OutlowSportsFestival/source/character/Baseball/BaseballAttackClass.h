#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "BaseballPlayer.h"
#include "../../utillity/LocusHDR.h"
#include <vector>

//****************************************************************
//		野球の攻撃クラス(COM,PLAYER共通)
//****************************************************************


class BaseballAttackClass
{
public:

	//攻撃操作クラス
	class ControllEvent
	{
	public:
		virtual ~ControllEvent(){}
		virtual bool isDoCombo() = 0;    //戻りとしてtrueを返すとコンボ実行をする
		virtual void AngleControll(RADIAN angle) = 0;//アングル変更を行えるフレームに呼ばれる
	};

	//攻撃情報クラス
	class AttackInfo
	{
	public:

		virtual ~AttackInfo(){}

		//ダメージパラメータをセット
		virtual void DamageParamSet(DamageShpere* pDmg) = 0;

		//ダメージ位置をセット
		virtual void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pTennis) = 0;

		//モーションをセット
		virtual void MotionSet(BaseballPlayer* pTennis) = 0;

		//引数のフレームが攻撃終了かどうか
		virtual bool isEnd(int Frame) = 0;

		//引数のフレームがコンボ移行フレームかどうか
		virtual bool isComboSwitchFrame(int Frame) = 0;

		//引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
		virtual bool isComboButtonFrame(int Frame) = 0;

		//引数のフレームが、角度調整を行えるフレームかどうか
		virtual bool isCanAngleControll(int Frame, RADIAN& OutControllVal) = 0;

		//引数のフレームが、ダメージ判定有効フレームかどうか
		virtual bool isDamageEnable(int Frame) = 0;

		//毎フレーム呼ばれる
		virtual void Update(int Frame, LocusHDR* pLocus) = 0;

		//攻撃があたったときに呼ばれる
		virtual void HitAttack(DamageShpere* pDmg) = 0;

        //カウンターヒットかどうか
        virtual bool isCounterHitFrame(int Frame) = 0;
	};

	typedef std::vector<AttackInfo*> AttackInfoArray;

	AttackInfoArray       m_AttackInfoArray; //攻撃情報クラス配列(入れた分だけコンボできる)


	BaseballAttackClass(
		BaseballPlayer*   pOwner,  //野球プレイヤーへのポインタ
		ControllEvent*  pEvent   //イベントクラス(終了時にdeleteする)
		);

	~BaseballAttackClass();

	bool getDoHit()
	{
		return m_DoHit;
	}

	void Update();	//更新

	bool isEnd()const;   //終了確認



private:

	BaseballPlayer*  const  m_pOwner;
	ControllEvent* const  m_pEvent;
	DamageShpere	      m_Damage;
	int                   m_Timer;
	int                   m_ComboCount;
	bool                  m_DoCombo;
	bool                  m_DoHit;
	LocusHDR              m_Locus;
	int                   m_DamageHitCount;
	int                   m_HitStopCount;

	void(BaseballAttackClass::*m_pStateFunc)();

	void State_Attack();
	void State_NextAtk();
	void State_End();

	bool isLastAtk();
};
