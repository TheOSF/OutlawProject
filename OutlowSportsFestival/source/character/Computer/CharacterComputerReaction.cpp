#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"
#include "../../Ball/UsualBall.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					    pParent,	//操るキャラクタのポインタ
    InParam                             param,
    ActionEvent*						pActionEvent
	) :
    m_pChr(pParent),
    m_pActionEvent(pActionEvent),
    m_DoActionCount(0),
    m_InParam(param),
    m_DamageVec(Vector3Zero),
    m_Reactioned(false)
{
    m_DontCheckCounter = 0;
    m_DontCheckNearEnemy = 0;
    m_DontCheckDamageEscape = 0;

    m_ReactionTypeParam.isBall = false;
    m_ReactionTypeParam.isCanCounter = false;
    m_ReactionTypeParam.isStrongDamage = false;
    m_ReactionTypeParam.isWeakDamage = false;
}


CharacterComputerReaction::~CharacterComputerReaction()
{
	delete m_pActionEvent;
}


void CharacterComputerReaction::Update()
{
    //すでに検知していたかどうか
    if (m_DoActionCount > 0)
    {
        //反応する
        if (--m_DoActionCount == 0)
        {
            m_Reactioned = true;
            m_pActionEvent->Reaction(m_ReactionTypeParam, m_DamageVec);
        }
    }
    else
    {
        if (m_DontCheckCounter <= 0)
        {
            //ボール検知
            CheckBall();
        }
        else
        {
            --m_DontCheckCounter;
        }
        
        if (m_DontCheckNearEnemy<=0)
        {
            //キャラクタ検知
            CheckNearCharacter();
        }
        else
        {
            --m_DontCheckNearEnemy;
        }
       
        if (m_DontCheckDamageEscape <= 0)
        {
            //ダメージ検知
            CheckDamage();
        }
        else
        {
            --m_DontCheckDamageEscape;
        }
    }
}

bool CharacterComputerReaction::Reactioned()const
{
    return m_Reactioned;
}

CharacterComputerReaction::StrongParam CharacterComputerReaction::GetParam()
{
    StrongParam ret;

    switch (m_pChr->m_PlayerInfo.strong_type)
    {
    case StrongType::_Weak:
        ret.CounterSuccess = 0.5f;
        ret.CounterReactionSpeed = 15.0f;

        ret.NearEnemySuccess = 0.5f;
        ret.NearEnemyReactionSpeed = 30;

        ret.DamageEscapeSuccess = 0.5f;
        ret.DamageEscapeReactionSpeed = 5.0f;
        break;

    case StrongType::_Usual:
        ret.CounterSuccess = 0.75f;
        ret.CounterReactionSpeed = 12.0f;

        ret.NearEnemySuccess = 0.75f;
        ret.NearEnemyReactionSpeed = 20;

        ret.DamageEscapeSuccess = 0.75f;
        ret.DamageEscapeReactionSpeed = 2.0f;
        break;

    case StrongType::_Strong:
        ret.CounterSuccess = 0.95f;
        ret.CounterReactionSpeed = 5.0f;

        ret.NearEnemySuccess = 0.9f;
        ret.NearEnemyReactionSpeed = 5;

        ret.DamageEscapeSuccess = 0.9f;
        ret.DamageEscapeReactionSpeed = 0.0f;
        break;

    default:
        MyAssert(false, "対応していない難易度タイプです");
        break;
    }

    return ret;
}

bool CharacterComputerReaction::CheckBall()
{
    BallBase* pCounterBall = nullptr;
    Vector3 pos = m_pChr->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    Vector3 out;

    //カウンター可能なボールを検出
    if (DefBallMgr.GetCounterBall(
        &pCounterBall,
        pos,
        &out,
        m_InParam.CounterAreaSize,
        1,
        m_pChr
        ))
    {
        if (frand() > GetParam().CounterSuccess)
        {
            //失敗！！
            m_DontCheckCounter = 30;
            return false;
        }

        //検出できた場合

        //反応フレーム設定
        m_DoActionCount = (int)(GetParam().CounterReactionSpeed * frand());

        //パラメータ設定
        m_ReactionTypeParam.isBall = true;
        m_ReactionTypeParam.isCanCounter = true;

        m_DamageVec = pCounterBall->m_Params.move;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckNearCharacter()
{
    CharacterBase* pNearCharacter = nullptr;

    if (chr_func::CalcAtkTarget(m_pChr, PI, m_InParam.CanNearAtkAreaSize, &pNearCharacter))
    {
        if (frand() > GetParam().NearEnemySuccess)
        {
            //失敗！！
            m_DontCheckNearEnemy = 30;
            return false;
        }

        //検出できた場合

        //反応フレーム設定
        m_DoActionCount = (int)(GetParam().NearEnemyReactionSpeed * frand());

        //パラメータ設定
        m_ReactionTypeParam.isEnemyNear = true;

        m_DamageVec = pNearCharacter->m_Params.pos - m_pChr->m_Params.pos;
        m_DamageVec.y = 0;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckDamage()
{
    SphereParam sp;

    sp.pos = m_pChr->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    sp.size = 5.0f; 

    CharacterComputerReactionHitEvent hitevent(m_pChr);

    //反応レーダー展開
    DefDamageMgr.HitCheckSphere(sp, hitevent);

    //検知できていた場合
    if (hitevent.HitDamage)
    {
        if (frand() > GetParam().NearEnemySuccess)
        {
            //失敗！！
            m_DontCheckNearEnemy = 30;
            return false;
        }

        //反応フレーム設定
        m_DoActionCount = (int)(GetParam().NearEnemyReactionSpeed * frand());

        //パラメータ設定
        Vector3 OutPos = Vector3Zero;
        hitevent.pDmg->CalcPosVec(m_pChr->m_Params.pos, &OutPos, &m_DamageVec);

        m_ReactionTypeParam.isBall = (hitevent.pDmg->pBall != nullptr);
        m_ReactionTypeParam.isCanCounter = (hitevent.hittype == CharacterComputerReactionHitEvent::HitType::CanCounter);
        m_ReactionTypeParam.isWeakDamage = (hitevent.pDmg->type == DamageBase::Type::_WeekDamage);
        m_ReactionTypeParam.isStrongDamage = !m_ReactionTypeParam.isWeakDamage;

        return true;

    }

    return false;
}