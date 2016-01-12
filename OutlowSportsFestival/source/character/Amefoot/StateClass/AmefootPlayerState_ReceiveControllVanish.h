#pragma once

#include "../AmefootPlayer.h"
#include "../../CharacterDamageControll.h"

//***************************************************
//		コントロール吹き飛びクラス
//***************************************************
class AmefootState_ControllVanish : public AmefootState
{
public:
    AmefootState_ControllVanish(AmefootPlayer*  pAmefoot);
    ~AmefootState_ControllVanish();

    //ゲッタ
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // ステート開始
    void Enter(AmefootPlayer* t)override;

    // ステート実行
    void Execute(AmefootPlayer* t)override;

    // ステート終了
    void Exit(AmefootPlayer* t)override;

private:
    AmefootPlayer*  const        m_pAmefoot;
    CharacterDamageControll*    m_pMoveClass;
};
