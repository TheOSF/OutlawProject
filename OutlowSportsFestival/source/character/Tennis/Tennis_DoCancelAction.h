#pragma once

#include "TennisPlayer.h"

#include "../Computer/CharacterComputerDoAction.h"
#include "../Computer/CharacterComputerReaction.h"

#include <bitset>

//----------------------------------------------------------------//
//  テニスキャラクタのキャンセルアクションのセットクラス
//----------------------------------------------------------------//


//ベースクラス
class Tennis_DoCancelAction
{
public:
    enum Action
    {
        NearAtk       ,//近距離攻撃
        FarAtk        ,//遠距離攻撃
        Step          ,//ステップ(回避
        SkillAtk      ,//必殺技
        BoundBallShot ,//特殊技
        Counter       ,//カウンター

        _MaxAction
    };

    Tennis_DoCancelAction(TennisPlayer* pTennis);
    virtual ~Tennis_DoCancelAction(){}

    void SetDontActions(Action ac);    //実行できないアクションのビットをセットする
    bool isCanAction(Action ac)const;  //引数のアクションが実行できるかどうか

    virtual bool DoAction() = 0;       //アクション実行(戻り値：他のアクションを実行したかどうか)

protected:
    TennisPlayer* const m_pTennis;

private:
    std::bitset<_MaxAction> m_Flags;

};


//プレイヤー用
class Tennis_DoCancelAction_Player :public Tennis_DoCancelAction
{
public:
    Tennis_DoCancelAction_Player(TennisPlayer* pTennis);
    bool DoAction()override;
};

//コンピューター用
class Tennis_DoCancelAction_Computer :public Tennis_DoCancelAction
{
public:

    Tennis_DoCancelAction_Computer(TennisPlayer* pTennis);
    ~Tennis_DoCancelAction_Computer();

    bool DoAction()override;

private:

    CharacterComputerReaction*    m_pReaction;
    CharacterComputerDoAction*    m_pDoAction;

};
