#include "LocusBarObject.h"

LocusBarObject::LocusBarObject(const InitParams& Param, UINT LocusLength) :
m_Locus(LocusLength),
m_DeleteTimer(0),
m_Vec(Vector3Normalize(Param.start_vec)),
m_Speed(Param.speed),
m_LocusPosLen(Param.LocusPosLen),
m_Count(0),
m_Origin(Param.origin)
{
    Vector3 Axis;

    Vector3Cross(Axis, Param.start_vec, Param.end_vec);
    Axis.Normalize();

    D3DXQuaternionRotationAxis(&m_Rot, &Vector3Trans<Vector3, D3DXVECTOR3>(Axis), Vector3Radian(Param.start_vec, Param.end_vec));
}

LocusBarObject::~LocusBarObject()
{
    
}

bool LocusBarObject::Update()
{
    {
        //�I���x�N�g���Ɍ������ĉ�]
        D3DXQUATERNION Identity, NowRot;
        Vector3 LocusAxis;

        D3DXQuaternionIdentity(&Identity);
        D3DXQuaternionSlerp(&NowRot, &Identity, &m_Rot, m_Count);

        LocusAxis = Vector3RotateQuaternion(NowRot, m_Vec);

        //���݂̈ʒu�ɓ_��ǉ�
        m_Locus.AddPoint(
            m_Origin + LocusAxis*m_LocusPosLen,
            LocusAxis
            );

        //�J�E���g�i�s
        m_Count += m_Speed;

        if (m_Count > 1.0f)
        {
            m_Count = 1.0f;
            ++m_DeleteTimer;
        }
    }

    return m_DeleteTimer < (UINT)m_Locus.GetNumPoint();
}

bool LocusBarObject::Msg(MsgType mt)
{

    return false;
}