#pragma once
#include "StagePhysicMoveObject.h"


//-----------------------------------------------------//
//  　ステージのオブジェクトを設定する関数集
//-----------------------------------------------------//

namespace StageObjFactory
{
    StagePhysicMoveObject* CreateCone(CrVector3 pos, CrVector3 angle);
    StagePhysicMoveObject* CreatePipe(CrVector3 pos, CrVector3 angle);
    StagePhysicMoveObject* CreateBench(CrVector3 pos, CrVector3 angle);
}