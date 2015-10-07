
#pragma once

#include "../../GameSystem/GameObject.h"
#include "BulletSystem.h"

//-------------------------------------------------------------------//
//      �����G���W�����X�V����Q�[���I�u�W�F�N�g
//-------------------------------------------------------------------//

class BulletUpdateGameobject :public GameObjectBase
{
public:
    BulletUpdateGameobject(){}
    ~BulletUpdateGameobject(){}

    
private:
    bool Update()override
    {
        //�X�V
        DefBulletSystem.StepSimulation(1.0f / 60.0f);

        return true;
    }

    bool Msg(MsgType mt)override
    {
        return true;
    }
};