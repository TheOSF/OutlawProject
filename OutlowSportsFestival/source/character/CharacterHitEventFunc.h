#pragma once

#include "../Damage/Damage.h"
#include "CharacterBase.h"


//************************************************************
//	キャラクタ　ダメージのひるみ分岐関数
//************************************************************

class CharacterHitEventFunc
{
public:

    enum class SetType
    {
        Die,      //死亡ステート
        Weak_Hit, //弱ひるみステート
        Vanish_Hit,//吹き飛びステート
        Controll_Hit,//吹き飛びコントロールステート

        _None, //何もしなくてよい
    };

    //ダメージ値 * m_DamageAddSkillValue分スキルゲージを加算する
    static const float m_DamageAddSkillValue;

    //戻り値：ひるむタイプ
    static SetType CheckDamage(
        DamageBase*    pDmg,   //当たったダメージへのポインタ
        CharacterBase* pOwner, //自身
        Vector3*       pOutVec //戻り値：ひるむ方向
        );
};
