#include "Item_BellEmitter.h"
#include "../character/CharacterManager.h"
#include "Item_Bell.h"
#include "Item_WallBoundBall.h"

Item_BellEmitter::Item_BellEmitter(UINT ApperFrame) :
m_ApperFrame((int)ApperFrame),
m_ApperCount(0),
m_FirstRound(false)
{

}

Item_BellEmitter::~Item_BellEmitter()
{

}

bool Item_BellEmitter::isCanApper()
{
    //出現可能かどうか
    return
        m_FirstRound == false &&
        m_ApperCount == m_ApperFrame &&
        DefCharacterMgr.GetCharacterLiveCount() > 2;
}

bool Item_BellEmitter::Update()
{
    m_ApperCount = min(m_ApperCount + 1, m_ApperFrame);

    //ベル出現タイミングかどうか
    if (isCanApper())
    {
        //出現
        new Item_Bell(
            3
            );


        //カウンタリセット
        m_ApperCount = 0;
    }

    return true;
}


bool Item_BellEmitter::Msg(MsgType mt)
{
    if (mt == MsgType::_RoundReset)
    {
        m_FirstRound = false;
        return true;
    }

    return false;
}