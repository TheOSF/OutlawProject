#pragma once

#include "../GameSystem/GameObject.h"

//------------------------------------------------------------//
//  �͐�~�ł̎ԁA�o�X�@�I�u�W�F�N�g�̐������s���N���X
//------------------------------------------------------------//

class StageCarEmitter :public GameObjectBase
{
public:
    StageCarEmitter(UINT round_frame);
    ~StageCarEmitter();


private:

    void(StageCarEmitter::*m_pStateFunc)();
    int m_ApperFrameCount;


    void State_ApperWait();   //���ł��Ԃ��o������
    void State_ApperAfter();  //��莞�ԑҋ@�̏��
    void State_Stop();        //�o�������Ȃ�

    void ApperCar(bool right);
    void ApperBus(int num);

    bool Update();
    bool Msg(MsgType mt);

};