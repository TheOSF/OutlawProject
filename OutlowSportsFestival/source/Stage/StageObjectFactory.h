#pragma once
#include "StagePhysicMoveObject.h"


//-----------------------------------------------------//
//  �@�X�e�[�W�̃I�u�W�F�N�g��ݒ肷��֐��W
//-----------------------------------------------------//

namespace StageObjFactory
{
    StagePhysicMoveObject* CreateCone(CrVector3 pos, CrVector3 angle);
    StagePhysicMoveObject* CreatePipe(CrVector3 pos, CrVector3 angle);
    StagePhysicMoveObject* CreateBench(CrVector3 pos, CrVector3 angle);
}