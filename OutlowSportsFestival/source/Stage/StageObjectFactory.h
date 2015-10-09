#pragma once

//-----------------------------------------------------//
//  　ステージのオブジェクトを設定する関数集
//-----------------------------------------------------//

namespace StageObjFactory
{
    void CreateCone(CrVector3 pos, CrVector3 angle);
    void CreatePipe(CrVector3 pos, CrVector3 angle);
    void CreateBench(CrVector3 pos, CrVector3 angle);
}