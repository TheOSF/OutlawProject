#pragma once

#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"

//---------------------------------------------------
// �@�@�@�@���G�t�F�N�g
//--------------------------------------------------- 

class ThunderEffect :public GameObjectBase
{
public:
    ThunderEffect(
        CrVector3 origin,     //�łn�_
        CrVector3 target,     //�I�_
        float     random,     //���ւ̃����_���ȁA�΂炯�x
        float     width,      //����
        float     live_time,  //���̎���
        Vector4   color,      //�F
        UINT      point_num   //�܂�Ȃ����ő吔�A�����Əd���̂�20���x���I�X�X��
        );

    ~ThunderEffect();

private:
    const float   m_LiveTime;
    float         m_LiveCount;
    LocusHDR      m_Locus;
    const UINT    m_Point_num;
    const Vector3 m_Origin, m_Target;
    const float   m_Random;
    Vector3       m_MoveVec;

    void SetNewPos();
    void UpdatePos();

    bool Update();
    bool Msg(MsgType mt);
};