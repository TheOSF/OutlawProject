
#pragma once

#include "../../GameSystem/GameObject.h"
#include "BulletSystem.h"

//-------------------------------------------------------------------//
//      物理エンジンを更新するゲームオブジェクト
//-------------------------------------------------------------------//

class BulletUpdateGameobject :public GameObjectBase
{
public:
    BulletUpdateGameobject(){}
    ~BulletUpdateGameobject(){}

    
private:
    bool Update()override
    {
        //更新
        DefBulletSystem.StepSimulation(1.0f / 60.0f);

        return true;
    }

    bool Msg(MsgType mt)override
    {
        return true;
    }
};