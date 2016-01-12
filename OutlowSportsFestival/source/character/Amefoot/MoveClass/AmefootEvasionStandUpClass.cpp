#include "AmefootEvasionStandUpClass.h"
#include "../AmefootPlayer.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"


//-----------------------------------------------------------------------------------------
// AmefootEvasionStandUpClass
// [ アメフト ] 回避後立ち上がるクラス
// アメフト専用なのでイベントを使用しない
//-----------------------------------------------------------------------------------------
AmefootEvasionStandUpClass::AmefootEvasionStandUpClass(
     AmefootPlayer* pAmefoot,
     Params params
     ) :
     m_pAmefoot(pAmefoot),
     m_timer(0)
{
    //周囲見るフラグがonなら
    if (params.isViewAround)
    {
        m_pStateFunc = &AmefootEvasionStandUpClass::State_ViewAround;
    }
    else
    {
        m_pStateFunc = &AmefootEvasionStandUpClass::State_StandUp;
    }

    
}
//-----------------------------------------------------------------------------------------
bool AmefootEvasionStandUpClass::Update()
{
    //更新＆生存フラグ取得
    bool ret = (this->*m_pStateFunc)();

    // モデルのワールド変換行列を更新
    chr_func::CreateTransMatrix(
        m_pAmefoot,
        &m_pAmefoot->m_Renderer.m_TransMatrix
        );

    return ret;
}
bool AmefootEvasionStandUpClass::State_ViewAround()
{
    const int AroundViewFrame = 28;

    //アニメーション更新
    m_pAmefoot->m_Renderer.Update(2.0f);

    if (m_timer == 0)
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    m_timer++;

    //時間経過で周囲きょろきょろ関数をセット
    if (m_timer >= AroundViewFrame)
    {
        m_timer = 0;
        m_pStateFunc = &AmefootEvasionStandUpClass::State_StandUp;
    }

    return true;
}

bool AmefootEvasionStandUpClass::State_StandUp()
{
    const int StandUpFrame = 50;

    //アニメーション更新
    m_pAmefoot->m_Renderer.Update(1);

    
    if (m_timer == 0)
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    m_timer++;

    if (m_timer >= StandUpFrame)
    {
        m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------


