#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"
#include "../../Ball/UsualBall.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					    pParent,	//操るキャラクタのポインタ
	const CharacterComputerMove::Param&	param,	
	ActionEvent*						pActionEvent
	) :
	m_pCharacter(pParent), 
    m_Params(param),
    m_pActionEvent(pActionEvent),
    m_DoActionCount(0)
{

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
            m_pActionEvent->Reaction(m_HitType, m_DamageVec);
        }
    }
    else
    {
        //ボール検知
        CheckBall();

        //ダメージ検知
        CheckDamage();

        //キャラクタ検知
        CheckNearCharacter();
    }
}

bool CharacterComputerReaction::CheckBall()
{
    BallBase* pCounterBall = nullptr;
    Vector3 pos = m_pCharacter->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    Vector3 out;

    //カウンター可能なボールを検出
    if (DefBallMgr.GetCounterBall(
        &pCounterBall,
        pos,
        &out,
        10.0f,
        5,
        m_pCharacter
        ))
    {
        //検出できた場合

        //反応フレーム設定
        m_DoActionCount = (int)(m_Params.BallCounterSpeed * 10.0f * frand());

        //パラメータ設定
        m_HitType = CharacterComputerReactionHitEvent::HitType::CanCounter;
        m_DamageVec = pCounterBall->m_Params.move;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckNearCharacter()
{
    CharacterBase* pNearCharacter = nullptr;

    if (chr_func::CalcAtkTarget(m_pCharacter, PI, 3.0f, &pNearCharacter) && m_Params.DangerEscape >= frand())
    {
        //検出できた場合

        //反応フレーム設定
        m_DoActionCount = 2;

        //パラメータ設定
        m_HitType = CharacterComputerReactionHitEvent::HitType::Escape;
        m_DamageVec = m_pCharacter->m_Params.pos - pNearCharacter->m_Params.pos;
        m_DamageVec.y = 0;
        m_DamageVec.Normalize();

        return true;
    }

    return false;
}

bool CharacterComputerReaction::CheckDamage()
{
    SphereParam sp;

    sp.pos = m_pCharacter->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
    sp.size = 5.0f; 

    CharacterComputerReactionHitEvent hitevent(m_pCharacter);

    //反応レーダー展開
    DefDamageMgr.HitCheckSphere(sp, hitevent);

    //検知できていた場合
    if (hitevent.HitDamage && m_Params.DangerEscape >= frand())
    {
        //反応フレーム設定
        m_DoActionCount = 1;

        //パラメータ設定
        Vector3 OutPos = Vector3Zero;
        hitevent.pDmg->CalcPosVec(m_pCharacter->m_Params.pos, &OutPos, &m_DamageVec);

        m_HitType = CharacterComputerReactionHitEvent::HitType::Escape;

        return true;

    }

    return false;
}