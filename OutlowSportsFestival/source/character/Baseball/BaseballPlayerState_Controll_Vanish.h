#pragma once

#include "BaseBallPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		コントロール吹き飛びクラス
//***************************************************
class BaseballState_ControllVanish : public BaseballState
{
public:
    BaseballState_ControllVanish(BaseballPlayer*  pBaseBall);
    ~BaseballState_ControllVanish();

    //ゲッタ
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // ステート開始
    void Enter(BaseballPlayer* p)override;

    // ステート実行
    void Execute(BaseballPlayer* p)override;

    // ステート終了
    void Exit(BaseballPlayer* p)override;

private:
    BaseballPlayer*  const        m_pBaseBall;
    CharacterDamageControll*      m_pMoveClass;
};
