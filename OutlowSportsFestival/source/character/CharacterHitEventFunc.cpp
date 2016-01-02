#include "CharacterHitEventFunc.h"
#include "CharacterFunction.h"


CharacterHitEventFunc::SetType CharacterHitEventFunc::CheckDamage(DamageBase* pDmg, CharacterBase* pOwner, Vector3* pOutVec)
{

    //自分の作っているダメージだった場合は何もしない
    if (pDmg->pParent != nullptr &&
        pDmg->pParent->m_PlayerInfo.number == pOwner->m_PlayerInfo.number || 
        pDmg->isCanHitCharacter(pOwner) == false
        )
    {
        return SetType::_None;
    }

    //ダメージのパラメータ
    Vector3 dmg_pos, dmg_vec;

    //ダメージ方向と位置を算出
    pDmg->CalcPosVec(pOwner->CalcHitCheckPos(), &dmg_pos, pOutVec);

    //ヒットカウント加算
    pDmg->AddHitCount(pOwner);

    //ダメージ計算
    chr_func::CalcDamage(pOwner, pDmg->Value, pDmg->isOptionOn(DamageBase::Option::_DontDie));


    //もし体力がなかったら、どんな攻撃であろうと死亡ステートへ
    if (chr_func::isDie(pOwner))
    {
        return SetType::Die;
    }


    //当たった時にそのダメージの種類から、それぞれのステートに派生させる
    switch (pDmg->type)
    {
    case DamageBase::Type::_WeekDamage:

        //地面に付いていた場合のみ
        if (chr_func::isTouchGround(pOwner))
        {
            //弱ひるみは上に移動しない
            pOutVec->y = 0;
            //弱ダメージ
            return SetType::Weak_Hit;
        }

        pOutVec->y = 0;

        //未作成
    case DamageBase::Type::_VanishDamage:
        //吹き飛びダメージ
        return SetType::Vanish_Hit;

    case DamageBase::Type::_ControllDamage:
        //吹き飛びコントロールダメージ
        return SetType::Controll_Hit;

    default:

        break;
    }

    return SetType::_None;
}
