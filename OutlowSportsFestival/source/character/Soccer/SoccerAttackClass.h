#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "SoccerPlayer.h"
#include "../../utillity/Locus.h"
#include <vector>

//****************************************************************
//		サッカーの攻撃クラス(COM,PLAYER共通)
//****************************************************************


class SoccerAttackClass
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
		virtual void DamagePosSet(DamageShpere* pDmg, SoccerPlayer* pSoccer) = 0;

		//モーションをセット
		virtual void MotionSet(SoccerPlayer* pSoccer) = 0;

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
		virtual void Update(int Frame, Locus* pLocus) = 0;

		//攻撃が当たった時に呼ばれる
		virtual void HitAttack(DamageShpere* pDmg) = 0;

		//ヒットストップかどうか
		virtual bool isHitStopFrame() = 0;

		//ヒットストップ時更新
		virtual void HitStopUpdate() = 0;
	};

	typedef std::vector<AttackInfo*> AttackInfoArray;

	AttackInfoArray       m_AttackInfoArray; //攻撃情報クラス配列(入れた分だけコンボできる)


	SoccerAttackClass(
		SoccerPlayer*   pOwner,  //サッカープレイヤーへのポインタ
		ControllEvent*  pEvent,   //イベントクラス(終了時にdeleteする)
		int NoDamageFrame
		);

	~SoccerAttackClass();
	bool getDoHit()
	{
		return m_DoHit;
	}
	

	void Update();	//更新

	bool isEnd()const;   //終了確認



private:

	SoccerPlayer*  const  m_pOwner;
	ControllEvent* const  m_pEvent;
	DamageShpere	      m_Damage;
	int                   m_Timer;
	int                   m_ComboCount;
	int                   NoDamageFrame;
	bool                  m_DoCombo;
	bool                  m_DoHit;
	Locus                 m_Locus;
	int                   m_DamageHitCount;
	int                   m_HitStopCount;

	void(SoccerAttackClass::*m_pStateFunc)();

	void State_Attack();
	void State_NextAtk();
	void State_End();

	bool isLastAtk();
};
