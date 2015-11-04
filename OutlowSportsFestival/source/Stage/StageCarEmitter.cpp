#include "StageCarEmitter.h"
#include "StageObjectCar.h"

#include "../debug/DebugDraw.h"


StageCarEmitter::StageCarEmitter(UINT round_frame) :
m_ApperFrameCount(0),
m_pStateFunc(&StageCarEmitter::State_Stop)
{

}

StageCarEmitter::~StageCarEmitter()
{

}




void StageCarEmitter::State_ApperWait()
{
    if (rand() % 100 != 0)
    {
        return;
    }

    //�Ԃ��o��
    ApperCar(rand() % 2 == 0);

    //�o���ҋ@�X�e�[�g��
    m_pStateFunc = &StageCarEmitter::State_ApperAfter;

    m_ApperFrameCount = 0;
}

void StageCarEmitter::State_ApperAfter()
{
    if (++m_ApperFrameCount > 120)
    {
        //�o���X�e�[�g��
        m_pStateFunc = &StageCarEmitter::State_ApperWait;
    }
}


void StageCarEmitter::State_Stop()
{
    //�������Ȃ�
}

void StageCarEmitter::ApperCar(bool right)
{
    Vector3 pos(130, 2.4f, 30), goal(-130, 2.4f, 30);

    if (right)
    {
        Vector3 tmp = pos;

        pos = goal;

        goal = tmp;
        
        pos.z = goal.z = 37.0f;
    }

/*
    new DebugDrawSphere(
        pos, 2, COLORf(0.5f,1,0,0),120
        );

    new DebugDrawSphere(
        goal, 2, COLORf(0.5f, 1, 0, 0), 120
        );*/


    new StageObjectCar(
        pos,
        goal,
        0.8f,
        StageObjectCar::CarType::Car1,
        true
        );
}


bool StageCarEmitter::Update()
{
    (this->*m_pStateFunc)();

    return true;
}


bool StageCarEmitter::Msg(MsgType mt)
{
    switch (mt)
    {
    case MsgType::_PlayStart:

        //�o���ҋ@�X�e�[�g��
        m_pStateFunc = &StageCarEmitter::State_ApperAfter;
        break;

    case MsgType::_GameSet:

        //�o���֎~�X�e�[�g��
        m_pStateFunc = &StageCarEmitter::State_Stop;
        break;

    default:
        break;
    }

    return false;
}