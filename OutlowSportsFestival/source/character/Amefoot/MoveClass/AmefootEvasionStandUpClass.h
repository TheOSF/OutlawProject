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
          int AllFrame;
     };

     // コンストラクタ
     AmefootEvasionStandUpClass(
          AmefootPlayer* pAmefoot,
          Params params);

     // 更新
     bool Update();

private:
     AmefootPlayer* m_pAmefoot;
     const Params m_params;
     int m_timer;

};

