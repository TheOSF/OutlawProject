#pragma once 

#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"

//----------------------------------------------------//
// �O�Ղ��~��ɕ��ׂ�I�u�W�F�N�g
//----------------------------------------------------//

class LocusBarObject :public GameObjectBase
{
public:
    struct InitParams
    {
        Vector3 origin;    //���_
        Vector3 start_vec; //�n�܂������_�ł̃x�N�g��
        Vector3 end_vec;   //�I�����_�ł̃x�N�g��

        RATIO   speed;     //��]�X�s�[�h

        float   LocusPosLen;//���_����O�Ղ܂ł̋���
    };

    LocusHDR  m_Locus;    

    LocusBarObject(
        const InitParams& Param,
        UINT              LocusLength
        );

    ~LocusBarObject();


    bool Update();
    bool Msg(MsgType mt);

private:
    const RATIO     m_Speed;
    const float     m_LocusPosLen;
    const Vector3   m_Vec;
    const Vector3   m_Origin;

    D3DXQUATERNION  m_Rot;
    RATIO           m_Count;
    UINT            m_DeleteTimer;
  
};