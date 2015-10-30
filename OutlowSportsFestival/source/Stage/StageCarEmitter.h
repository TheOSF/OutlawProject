#pragma once

#include "../GameSystem/GameObject.h"

//------------------------------------------------------------//
//  河川敷での車、バス　オブジェクトの生成を行うクラス
//------------------------------------------------------------//

class StageCarEmitter :public GameObjectBase
{
public:
    StageCarEmitter(UINT round_frame);
    ~StageCarEmitter();


private:

    void(StageCarEmitter::*m_pStateFunc)();
    int m_ApperFrameCount;


    void State_ApperWait();   //いつでも車を出せる状態
    void State_ApperAfter();  //一定時間待機の状態
    void State_Stop();        //出現させない

    void ApperCar(bool right);
    void ApperBus(int num);

    bool Update();
    bool Msg(MsgType mt);

};