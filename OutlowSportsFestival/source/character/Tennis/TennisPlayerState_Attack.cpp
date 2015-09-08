#include "TennisPlayerState_Attack.h"

#include "TennisPlayerState_UsualMove.h"
#include "../Attack/CharacterAttack.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"


struct TennisComboParam
{
    CharacterNearAttack::Params AttackParams;
    int     Motion;
};

static const TennisComboParam TennisAttackParams[] =
{
    { { 12, 35, 0.1f, 0, 0.1f, 25, 35, 3, 1 }, TennisPlayer::_mt_Attack1 }, 
    { { 12, 20, 0.1f, 0, 0.1f, 12, 20, 3, 1 }, TennisPlayer::_mt_Attack2 },
    { { 12, 35, 0.1f, 0, 0.1f, 15, 35, 3, 1 }, TennisPlayer::_mt_Attack3 },
};

TennisState_PlayerControll_Attack::TennisState_PlayerControll_Attack() :
m_ComboNum((UINT)ARRAYSIZE(TennisAttackParams)), 
m_ComboCount(0),
m_pAttack(nullptr),
m_DoNextAttack(false)
{

}

TennisState_PlayerControll_Attack::~TennisState_PlayerControll_Attack()
{

}

// ステート開始
void TennisState_PlayerControll_Attack::Enter(TennisPlayer* t)
{
    //攻撃クラスを設定
    SetNextAttack(t);

}

// ステート実行中
void TennisState_PlayerControll_Attack::Execute(TennisPlayer* t)
{
    //スティック値を更新
    m_pAttack->SetStickValue(controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number));

    //コンボ移行フラグの更新
    if (m_pAttack->CanDoCombo() && controller::GetPush(controller::button::shikaku, t->m_PlayerInfo.number))
    {
        m_DoNextAttack = true;
    }

    //次のコンボに移行
    if (m_pAttack->CanGoNextCombo() && m_DoNextAttack)
    {
        SetNextAttack(t);
        m_DoNextAttack = false;
    }

    //更新が失敗（攻撃が終了）
    if (m_pAttack->Update() == false)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //ワールド変換行列
    chr_func::CreateTransMatrix(t, 0.05f, &t->m_Renderer.m_TransMatrix);
}

// ステート終了
void TennisState_PlayerControll_Attack::Exit(TennisPlayer* t)
{
    delete m_pAttack;
}


//次の攻撃クラスをセット
bool TennisState_PlayerControll_Attack::SetNextAttack(TennisPlayer* t)
{

    class TennisAttackEvent :public CharacterNearAttack::AttackEvent
    {
        TennisPlayer* m_pTennis;
        const int m_AttackMotion;
    public:
        TennisAttackEvent(TennisPlayer* pTennis, int AttackMotion) :
            m_pTennis(pTennis),
            m_AttackMotion(AttackMotion)
        {

        }

        void Update()
        {
            m_pTennis->m_Renderer.Update(1);
        }

        void AttackStart()
        {
            m_pTennis->m_Renderer.SetMotion(m_AttackMotion);
        }

        void AttackEnd()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
        }

        void Assault()
        {
            
        }
    };

    //最終コンボの場合は処理せずに終了
    if (m_ComboCount >= m_ComboNum)
    {
        return false;
    }

    const TennisComboParam& param = TennisAttackParams[m_ComboCount];

    //今までの攻撃クラスを削除
    delete m_pAttack;
    
    //攻撃クラスの作成
    m_pAttack = new CharacterNearAttack(
        t,
        param.AttackParams,
        new TennisAttackEvent(t, param.Motion),
        (m_ComboCount + 1 == m_ComboNum) ? (DamageBase::Type::_VanishDamage) : (DamageBase::Type::_WeekDamage),
        1
       );

    //コンボカウントを加算
    ++m_ComboCount;

    return true;
}