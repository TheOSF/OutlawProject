#pragma once

#include "../GameSystem/GameObject.h"

//---------------------------------------------------------//
// �@�X�e�[�W�̉��p�[�e�B�N�����o��������N���X
//---------------------------------------------------------//

class StageSmokeEmitter :public GameObjectBase
{
public:
    StageSmokeEmitter(
        CrVector3 minPos,     //�`�`�a�a�̍��W�������Ƃ��������n�_
        CrVector3 maxPos,     //�`�`�a�a�̍��W�������Ƃ��傫���n�_
        DWORD     Color,      //���̐F
        float     EmitteTime, //�o���p�x
        float     Random      //�o���̎��Ԃ̌덷
        );

    ~StageSmokeEmitter();

    bool Update();
    bool Msg(MsgType mt);

private:
    const Vector3   m_MinPos;
    const Vector3   m_MaxPos;
    const DWORD     m_Color;
    const float     m_EmitteTime;
    const float     m_Random;

    float           m_Count;
};