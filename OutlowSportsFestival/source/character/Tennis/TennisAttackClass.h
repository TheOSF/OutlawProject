#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "TennisPlayer.h"
#include "../../utillity/LocusHDR.h"
#include <vector>

//****************************************************************
//		テニスの攻撃クラス(COM,PLAYER共通)
//****************************************************************


class TennisAttackClass
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
        virtual void DamagePosSet(DamageShpere* pDmg, TennisPlayer* pTennis) = 0;

        //モーションをセット
        virtual void MotionSet(TennisPlayer* pTennis) = 0;

        //引数のフレームが攻撃終了かどうか
        virtual bool isEnd(int Frame) = 0;

        //引数のフレームがコンボ移行フレームかどうか
        virtual bool isComboSwitchFrame(int Frame) = 0;

        //引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
        virtual bool isComboButtonFrame(int Frame) = 0;

        //引数のフレームが、角度調整を行えるフレームかどうか
        virtual bool isCanAngleControll(int Frame,RADIAN& OutControllVal) = 0;

        //引数のフレームが、ダメージ判定有効フレームかどうか
        virtual bool isDamageEnable(int Frame) = 0;

        //毎フレーム呼ばれる
        virtual void Update(int Frame, LocusHDR* pLocus) = 0;

        //攻撃があたったときに呼ばれる
        virtual void HitAttack(DamageShpere* pDmg) = 0;
    };

    typedef std::vector<AttackInfo*> AttackInfoArray;
    
    AttackInfoArray       m_AttackInfoArray; //攻撃情報クラス配列(入れた分だけコンボできる)


    TennisAttackClass(
        TennisPlayer*   pOwner,  //テニスプレイヤーへのポインタ
        ControllEvent*  pEvent   //イベントクラス(終了時にdeleteする)
        );

    ~TennisAttackClass();

    void Update();	//更新

    bool isEnd()const;   //終了確認

  
    
private:

    TennisPlayer*  const  m_pOwner;
    ControllEvent* const  m_pEvent;
    DamageShpere	      m_Damage;
    int                   m_Timer;
    int                   m_ComboCount;
    bool                  m_DoCombo;
    LocusHDR              m_Locus;
    int                   m_DamageHitCount;

    void(TennisAttackClass::*m_pStateFunc)();

    void State_Attack();
    void State_NextAtk();
    void State_End();

    bool isLastAtk();
};
