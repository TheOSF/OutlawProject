#include "TornadoEffect.h"
#include "../GameSystem/ResourceManager.h"
#include "EffectFactory.h"

TornadoEffect::TornadoEffect(
    const Param&   InitParam,
    UINT           NumLocus,
    UINT           NumLocusPoint
    ) :
    m_LocusPoint(NumLocusPoint),
    m_NumLocus(NumLocus),
    m_Destroy(false),
    m_Angle(0)
{
    m_Param = InitParam;

    m_ppLocusPtrArray = new LocusHDR*[m_NumLocus];

    for (UINT i = 0; i < m_NumLocus; ++i)
    {
        m_ppLocusPtrArray[i] = new LocusHDR(m_LocusPoint);

        m_ppLocusPtrArray[i]->m_StartParam.Width = m_Param.LocusWidthEnd;
        m_ppLocusPtrArray[i]->m_StartParam.Color = Vector4(1, 1, 1, 0.1f);
        m_ppLocusPtrArray[i]->m_StartParam.HDRColor = Vector4(1, 1, 1, 1)*0.3f;

        m_ppLocusPtrArray[i]->m_EndParam.Width = m_Param.LocusWidthStart;
        m_ppLocusPtrArray[i]->m_EndParam.Color = Vector4(1, 1, 1, 0.9f);
        m_ppLocusPtrArray[i]->m_EndParam.HDRColor = Vector4(1, 1, 1, 1)*0.9f;

        m_ppLocusPtrArray[i]->m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
    }
}

TornadoEffect::~TornadoEffect()
{
    for (UINT i = 0; i < m_NumLocus; ++i)
    {
        delete m_ppLocusPtrArray[i];
    }

    delete[] m_ppLocusPtrArray;
}

void TornadoEffect::Destroy()
{
    m_Destroy = true;
}

bool TornadoEffect::Update()
{
    const RADIAN angle = (PI * 2) / (float)m_NumLocus;
    const float one_point_len = m_Param.Length / (float)m_LocusPoint;
    //const float one_width = (m_Param.maxWidth - m_Param.minWidth) / (float)m_LocusPoint;
   
    float one_width = 0.0f;
    float t = 0.0f;

    UINT i, j;
    Vector3 pos;
    D3DXQUATERNION rotate_q, total_q, locus_q, locus_point_q;


    m_Param.vec.Normalize();
    m_Param.right.Normalize();


    //��̃N�H�[�^�I���쐬
    D3DXQuaternionRotationAxis(&total_q, &D3DXVECTOR3(m_Param.vec.x, m_Param.vec.y, m_Param.vec.z), m_Angle);

    //�����̒��S���łP�O�Օ���]����N�H�[�^�I���쐬
    D3DXQuaternionRotationAxis(&rotate_q, &D3DXVECTOR3(m_Param.vec.x, m_Param.vec.y, m_Param.vec.z), angle);

    //�����̒��S���ŋO�Ղ̂P�n�_����]����N�H�[�^�I���쐬
    D3DXQuaternionRotationAxis(&locus_point_q, &D3DXVECTOR3(m_Param.vec.x, m_Param.vec.y, m_Param.vec.z), angle * 0.33f);

    for (i = 0; i < m_NumLocus; ++i)
    {
        locus_q = total_q;

        //�O�Ղ̈ʒu�������Z�b�g
        m_ppLocusPtrArray[i]->Clear();

        //�_��z�u
        for (j = 0; j < m_LocusPoint; ++j)
        {
            //��]�����O�Ղ̂ł͂��߂�ʒu���Z�o
            pos = Vector3RotateQuaternion(locus_q, m_Param.right);

            //�L������v�Z
            {
                if (j >= m_Param.middle_height)
                {
                    t = (float)(j - m_Param.middle_height) / (float)(m_LocusPoint - m_Param.middle_height - 1);
                    one_width = m_Param.middleWidth * (1 - t) + m_Param.highWidth*t;
                }
                else
                {
                    t = (float)j / (float)m_Param.middle_height;
                    one_width = m_Param.lowWidth*(1 - t) + m_Param.middleWidth * t;
                }
                

                pos *= one_width *(frand()*0.2f + 0.8f);
            }

            //�L�т��v�Z
            pos += m_Param.vec * one_point_len*(float)j;

            //���W�Ɏ����Ă���
            pos += m_Param.pos;


            //�_��ǉ�
            m_ppLocusPtrArray[i]->AddPoint(
                pos, 
                m_Param.vec  //�L����x�N�g���͗����̌���
                );

            //�o���ʒu����]
            locus_q *= locus_point_q;
        }

        //�O�Ղ̌��_�ʒu����]
        total_q *= rotate_q;
    }
    

    //�p�x���X�V
    m_Angle += m_Param.RotateSpeed;
    
    if (fabsf(m_Angle) > 1000)
    {
        m_Angle = 0;
    }

    return !m_Destroy;
}

bool TornadoEffect::Msg(MsgType msg)
{
    return false;
}