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

    bool SetState(AmefootState* state, int important = 0);

    void SetDontThrowBallTimer(int Frame);

    bool isCanThrowBall()const;

public:
    enum MotionType
    {
        Motion_Zero = 0,
        Motion_Stand = 3, // 待機
        Motion_Run = 4, // 走り
        Motion_Run_End = 6, // 走り終わり
        Motion_Evasion_Start = 7, // 回避しゃがみ
        Motion_Evasion_ViewAround = 8,//周りをきょろきょろ
        Motion_Evasion_Standup = 9, // 回避起き上がり
        Motion_Throw_Ball = 10, // ボール投げ
        Motion_Tackle_Save_Power = 12, // タックル溜め
        Motion_Tackle_Charge = 13, // タックル突進
        Motion_Tackle_Drive_Away = 14, // タックル打ち上げ
        Motion_Tackle_Touchdown = 16, // タックルタッチダウン
        Motion_Tackle_Touchdown_andStand = 17, // タックルタッチダウン
        //  Motion_Tackle_Standup, // タックル起き上がり
        Motion_Counter_Pose = 19, // カウンター構え
        Motion_Counter_Catch_Right = 21, // カウンター右キャッチ
        Motion_Counter_Catch_Left = 23, // カウンター左キャッチ
        Motion_Counter_Throw_Ball = 25, // カウンター投げ
        Motion_Counter_Failed = 38 , // カウンター失敗  ------------------------
        Motion_Damage_Weak = 40, // 弱攻撃ダメージ ------------------------
        Motion_Damage_Vanish_Start = 28, // 吹き飛びダメージ開始
        Motion_Damage_Vanish_Landing = 29, // 吹き飛びダメージ着地
        Motion_Damage_Vanish_Standup = 30, // 吹き飛びダメージ起き上がり
        Motion_Damage_Vanish_Hit_Wall_Fall = 32, // 吹き飛びダメージ壁落下
        Motion_Damage_Vanish_Hit_Wall_Landing = 33, // 吹き飛びダメージ壁地面ではねる
        Motion_Damage_Vanish_Hit_Wall_Standup = 34, // 吹き飛びダメージ壁起き上がり
        Motion_Touchdown_Start = 36, // 固有技開始
        Motion_Touchdown_Finish = 37, // 固有技球掴みからフィニッシュまで
        Motion_Touchdown_Failed = 38, // 固有技失敗
        Motion_Touchdown_Failed_Bounced = 40, // 固有技失敗 はじき返される
        Motion_Special_Attack_Start = 42, // 必殺技開始
        Motion_Special_Attack_Running = 43, // 必殺技走り
        Motion_Special_Attack_Finish = 44 , // 必殺技フィニッシュ

        Motion_Win_Pose = 0, // 勝利ポーズ
        Motion_Lose_Pose = 0, // 負けポーズ
    };

private:

    AmefootStateMachine*    m_pStateMachine;
    int                     m_DontThrowBallTimer;

};
//-----------------------------------------------------------------------------------------

