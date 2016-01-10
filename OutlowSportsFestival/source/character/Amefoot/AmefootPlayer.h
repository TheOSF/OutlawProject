#pragma once

#include "../CharacterBase.h"
#include "../CharacterStateTemplate.h"
#include "../CharacterRenderer.h"

class AmefootPlayer;
typedef ChrStateMachine<AmefootPlayer, CharacterBase::MsgType> AmefootStateMachine;
typedef ChrState<AmefootPlayer, CharacterBase::MsgType> AmefootState;


//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ アメフト ] キャラクタークラス
//-----------------------------------------------------------------------------------------
class AmefootPlayer : public CharacterBase
{
public:
    AmefootPlayer(const PlayerInfo& info);

    ~AmefootPlayer();

    bool Update()override;

    bool CharacterMsg(MsgType msg)override;

    void SetState(AmefootState* state, bool important = false);


public:
    enum MotionType
    {
        Motion_Zero = 0,
        Motion_Stand, // 待機
        Motion_Run_Start, // 走り出し
        Motion_Run_End, // 走り終わり
        Motion_Evasion_Start, // 回避しゃがみ
        Motion_Evasion_Standup, // 回避起き上がり
        Motion_Throw_Ball, // ボール投げ
        Motion_Tackle_Save_Power, // タックル溜め
        Motion_Tackle_Charge, // タックル突進
        Motion_Tackle_Drive_Away, // タックル打ち上げ
        Motion_Tackle_Touchdown, // タックルタッチダウン
        Motion_Tackle_Standup, // タックル起き上がり
        Motion_Counter_Pose, // カウンター構え
        Motion_Counter_Catch_Right, // カウンター右キャッチ
        Motion_Counter_Catch_Left, // カウンター左キャッチ
        Motion_Counter_Throw_Ball, // カウンター投げ
        Motion_Counter_Failed, // カウンター失敗
        Motion_Damage_Weak, // 弱攻撃ダメージ
        Motion_Damage_Vanish_Start, // 吹き飛びダメージ開始
        Motion_Damage_Vanish_Landing, // 吹き飛びダメージ着地
        Motion_Damage_Vanish_Standup, // 吹き飛びダメージ起き上がり
        Motion_Damage_Vanish_Hit_Wall, // 吹き飛びダメージ壁張り付き
        Motion_Damage_Vanish_Hit_Wall_Fall, // 吹き飛びダメージ壁落下
        Motion_Damage_Vanish_Hit_Wall_Landing, // 吹き飛びダメージ壁地面ではねる
        Motion_Damage_Vanish_Hit_Wall_Standup, // 吹き飛びダメージ壁起き上がり
        Motion_Touchdown_Start, // 固有技開始
        Motion_Touchdown_Finish, // 固有技球掴みからフィニッシュまで
        Motion_Touchdown_Failed, // 固有技失敗
        Motion_Touchdown_Failed_Bounced, // 固有技失敗 はじき返される
        Motion_Special_Attack_Start, // 必殺技開始
        Motion_Special_Attack_Running, // 必殺技走り
        Motion_Special_Attack_Finish, // 必殺技フィニッシュ

        Motion_Win_Pose = 0, // 勝利ポーズ
        Motion_Lose_Pose = 0, // 負けポーズ
    };

private:
    AmefootStateMachine* m_pStateMachine;


};
//-----------------------------------------------------------------------------------------

