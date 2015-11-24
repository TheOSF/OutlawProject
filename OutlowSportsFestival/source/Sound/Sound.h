#pragma once

//------------------------------------------------------------//
//  　　　　　音の再生・読み込みをする関数
//------------------------------------------------------------//

namespace Sound
{
    enum Type
    {
        Damage1, //ダメージＳＥ１
        Damage2, //ダメージＳＥ２
        Damage3, //ダメージＳＥ３

        AtkHit1, //キーン
        AtkHit2, //パーン

        Swing1,  //旧ゴルフ　近距離攻撃音
        Swing2,  //旧サッカー近距離攻撃音
        Swing3,  //旧テニス　近距離攻撃音

        Impact1, //衝撃音１(ドン)
        Impact2, //衝撃音２(ドカン)

        Sand1,   //砂の音
        Sand2,   //砂の音(ズザー)

        Counter, //カウンター音
        Explode, //爆発(旧サッカー必殺技の爆発)
        Skill,   //スキル技発動(シュイーン)
        Beam1,    //ビーム音(前作の野球必殺技)
        Beam2,   //ビーム音(前作のサッカー必殺技)

        Tennis_BallAtk,  //テニスボール打ち音
        BaseBall_kaki_n, //野球(カキーン)
		BaseBall_SP,     //野球(必殺)
        Golf_Swing,      //旧ゴルフ遠距離攻撃

        Cursor_move,  //カーソル移動時
        Cursor_enter, //カーソル決定
        Cursor_cancel,//カーソルキャンセル

        Kira_n,     //ラウンドポイント取得音

        Gong_Start, //ゴング音スタート
        Gong_End,   //ゴング音エンド

        Scene_Enter,//シーン移行時

		Change,//　野球切り替え時

		Soccer_Brake,//サッカーのブレーキ
		Soccer_Speed_Up1,//サッカーのダッシュ
		Soccer_Speed_Up2,//サッカーのダッシュ案２

        // BGM 

        _BGM_REGIST_START,   //この下からＢＧＭ

        BGM_Title,
        BGM_Option,
        BGM_ChrSelect,

        BGM_Battle,
        BGM_LastRound,

        _BGM_REGIST_END,     //ここまで
    };

    //読み込み
    void Initialize();

    //再生
    void Play(Type type, RATIO volume = 1, bool loop = false);

    //ＢＧＭストップ
    void StopBGM();
}