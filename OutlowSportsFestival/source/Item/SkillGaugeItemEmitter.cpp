#include "SkillGaugeUpItem.h"
#include "SkillGaugeItemEmitter.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterFunction.h"

SkillGaugeItemEmitter::SkillGaugeItemEmitter(int ApperAvrFrame) :
m_isEmmite(false),
m_EmitteCount(0),
m_ApperAvrFrame(ApperAvrFrame),
m_LoseEmitte(false)
{

}

SkillGaugeItemEmitter::~SkillGaugeItemEmitter()
{

}

bool SkillGaugeItemEmitter::CheckLosePlayer(CharacterBase** ppOut)
{
    //平均よりも一定以下の体力のプレイヤーを算出
    const RATIO MinRatio = 0.2f;
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    RATIO avg = 0, MostMin = 1, Temp;
    int Count = 0;

    //戻り値を初期化
    *ppOut = nullptr;

    for (auto it : ChrMap)
    {
        //死んでいるキャラを除外
        if (chr_func::isDie(it.first))
        {
            continue;
        }
        avg += chr_func::GetLifeRatio(it.first);
        Count++;
    }

    if (Count == 0)
    {
        return false;
    }

    avg /= Count;

    //平均より少ししたを
    MostMin = avg - MinRatio;

    for (auto it : ChrMap)
    {
        //死んでいるキャラを除外
        if (chr_func::isDie(it.first))
        {
            continue;
        }
        Temp = chr_func::GetLifeRatio(it.first);
        
        if (Temp < MostMin)
        {
            MostMin = Temp;
            *ppOut = it.first;
        }
    }

    return (*ppOut) != nullptr;
}

void SkillGaugeItemEmitter::SetApperFrame()
{
    m_EmitteCount = m_ApperAvrFrame + rand() % (60 * 60 * 1);
}

bool SkillGaugeItemEmitter::Update()
{
#if 1
    if (m_isEmmite == false)
    {
        return true;
    }

    CharacterBase* pTarget;



    if (m_LoseEmitte == false && CheckLosePlayer(&pTarget))
    {
        Vector3 from = Vector3(0, 20, 0);

        new SkillGaugeUpItem(
            from,
            Vector3Normalize(pTarget->m_Params.pos - from)*1.5 + Vector3Rand()*1.0f
            );
        
        m_LoseEmitte = true;

        SetApperFrame();
    }

    if (m_EmitteCount == 0)
    {
        new SkillGaugeUpItem(
            Vector3(0, 20, 0),
            Vector3Normalize(Vector3Rand())*0.5
            );

        SetApperFrame();
    }

    m_EmitteCount = max(0, m_EmitteCount - 1);
#else

    if (rand() % 240 == 0)
    {

        new SkillGaugeUpItem(
            Vector3(0, 20, 0),
            Vector3Normalize(Vector3Rand())*0.5
            );
    }
#endif
    
    return true;
}

bool SkillGaugeItemEmitter::Msg(MsgType mt)
{
    switch (mt)
    {
    case MsgType::_GameSet:
        m_isEmmite = false;
        return true;


    case MsgType::_RoundReset:
        m_isEmmite = false;
        return true;



    case MsgType::_PlayStart:
        m_isEmmite = true;
        m_LoseEmitte = false;
        SetApperFrame();
        return true;

    }

    return false;
}