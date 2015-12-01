#include "Item_BellEmitter.h"
#include "../character/CharacterManager.h"
#include "Item_Bell.h"
#include "Item_WallBoundBall.h"

Item_BellEmitter::Item_BellEmitter(UINT ApperFrame) :
m_ApperFrame((int)ApperFrame),
m_ApperCount(ApperFrame),
m_FirstRound(false)
{

}

Item_BellEmitter::~Item_BellEmitter()
{

}

bool Item_BellEmitter::isCanApper()
{
    //�o���\���ǂ���
    return
        m_FirstRound == false &&
        m_ApperCount == m_ApperFrame &&
        DefCharacterMgr.GetCharacterLiveCount() > 2;
}

bool Item_BellEmitter::Update()
{
    m_ApperCount = min(m_ApperCount + 1, m_ApperFrame);

    //�x���o���^�C�~���O���ǂ���
    if (isCanApper())
    {
        //�o��
        new Item_Bell(
            3
            );

        for (int i = 0; i < 3; ++i)
        {

            //�{�[��
            new Item_WallBoundBall(
                Vector3(0, 20, 10),
                Vector3Normalize(Vector3(frand(), 0, frand()))*0.65f, 
                Item_Bell::BellFreezePos
                );

        }

        //�J�E���^���Z�b�g
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