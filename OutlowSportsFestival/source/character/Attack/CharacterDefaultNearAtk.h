#ifndef __CHARACTER_ATTACK_CLASS_H__
#define __CHARACTER_ATTACK_CLASS_H__

class CharacterBase;

#include "iextreme.h"
#include "Damage/Damage.h"

//****************************************************************
//		キャラクタ共通の攻撃クラス
//****************************************************************


//通常攻撃クラス
class CharacterDefaultNearAttack
{
public:
    struct Params
    {
        int   EndFrame;          //攻撃終了フレーム

        Vector3 DamagePosOffset; //当たり判定の位置(キャラクタ空間上)

        int   DamageStartFrame;  //あたり判定出現フレーム
        int   DamageEndFrame;    //あたり判定消滅フレーム
    };

    class AttackEvent
    {
    public:
        virtual~AttackEvent(){}

        virtual void Start() = 0; //クラス開始時に呼ばれる
        virtual void End() = 0;   //クラス終了時に呼ばれる

        virtual void Update(int frame) = 0;	//毎フレーム実行される関数(引数：現在のフレーム)

        virtual void DamageStart() = 0;			//あたり判定出現フレームに呼ばれる関数
        virtual void DamageUpdate() = 0;        //あたり判定が出現している間に呼ばれる関数
        virtual void DamageEnd() = 0;			//あたり判定消滅フレームに呼ばれる関数
    };

    DamageShpere	m_Damage;

    CharacterDefaultNearAttack(
        CharacterBase*	pParent,	//操るキャラクタのポインタ
        const Params&	param,
        AttackEvent*	pAtttackEvent
        );

    ~CharacterDefaultNearAttack();

    void Update();	//更新

private:
    CharacterBase*	m_pCharacter;
    AttackEvent*	m_pAttackEvent;
    Params			m_Params;
    int             m_timer;

    void UpdateDamagePos();
};

#endif