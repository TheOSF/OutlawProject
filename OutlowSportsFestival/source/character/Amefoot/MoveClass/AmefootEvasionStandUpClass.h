#pragma once


class AmefootPlayer;
//-----------------------------------------------------------------------------------------
// AmefootEvasionStandUpClass
// [ アメフト ] 回避後立ち上がるクラス
// アメフト専用なのでイベントを使用しない
//-----------------------------------------------------------------------------------------
class AmefootEvasionStandUpClass
{
public:
     struct Params
     {
         bool isViewAround;
     };

     // コンストラクタ
     AmefootEvasionStandUpClass(
          AmefootPlayer* pAmefoot,
          Params params);

     // 更新
     bool Update();

private:
    bool(AmefootEvasionStandUpClass::*m_pStateFunc)();

     AmefootPlayer* m_pAmefoot;
     int m_timer;

     bool State_ViewAround();
     bool State_StandUp();
};

