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
     m_params(params),
     m_timer(0)
{}
//-----------------------------------------------------------------------------------------
bool AmefootEvasionStandUpClass::Update()
{
     if ( m_timer == 0 )
     {
          m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
     }

     m_timer++;

     if ( m_timer >= m_params.AllFrame )
     {
          m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
          return false;
     }

     m_pAmefoot->m_Renderer.Update(1);

     // モデルのワールド変換行列を更新
     chr_func::CreateTransMatrix(
          m_pAmefoot,
          m_pAmefoot->m_ModelSize,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );

     return true;
}
//-----------------------------------------------------------------------------------------


