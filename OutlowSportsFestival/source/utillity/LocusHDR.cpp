#include "LocusHDR.h"
#include "../Camera/Camera.h"

LocusHDR::LocusHDR(size_t SavePointNum) :
m_NumPoint(SavePointNum),
m_UseCount(0)
{
    //�|�C���g�f�[�^����
    m_pPointData = new Point[SavePointNum];

    //�p�����[�^������
    Initialize();
}

LocusHDR::~LocusHDR()
{
    //��@
    delete[] m_pPointData;
}

//�т̓_��ǉ�����
void LocusHDR::AddPoint(
    CrVector3 pos,  //���S�ʒu
    CrVector3 vec   //�т̐L�т����(���K�����Ă��Ȃ���΂Ȃ�Ȃ�)
    )
{
    //�ЂƂ��ɂ��炷
    for (int i = m_UseCount; i >= 0; --i)
    {
        m_pPointData[i] = m_pPointData[i - 1];
    }

    m_pPointData[0].pos = pos;
    m_pPointData[0].vec = vec;

    m_UseCount = min(m_UseCount + 1, m_NumPoint - 1);
}

//�т̒������O��
void LocusHDR::Clear()
{
    //�g�p�J�E���g���O��
    m_UseCount = 0;
}

//�O�Ղ̓_����납���������
void LocusHDR::SubPoint(
    size_t num      //��������_�̐�
    )
{
    //�g�p���Ă���_���������������ꍇ�͑S����
    if (m_UseCount <= num)
    {
        Clear();
        return;
    }

    //�g�p�J�E���g������
    m_UseCount -= num;
}

//�_�𓾂�(�Ȃ����false��Ԃ�)
bool LocusHDR::GetPos(UINT n, Vector3& out)const
{
    if (n >= m_UseCount)
    {
        return false;
    }

    out = m_pPointData[n].pos;

    return true;
}

//������
void LocusHDR::Initialize()
{
    m_Visible = true;
    m_pTexture = nullptr;

    m_StartParam.Width = 1;
    m_StartParam.Color = D3DXVECTOR4(1, 1, 1, 1);
    m_StartParam.HDRColor = D3DXVECTOR4(1, 1, 1, 1);

    m_EndParam.Width = 1;
    m_EndParam.Color = D3DXVECTOR4(1, 1, 1, 1);
    m_EndParam.HDRColor = D3DXVECTOR4(0, 0, 0, 0);

    m_Division = 4;
}

//�J���������Z�������v�Z����
void LocusHDR::CalcZ()
{
    m_SortZ = 1; //��
}


void LocusHDR::Render()
{
    //�`�悵�Ȃ��ꍇ
    if (!m_Visible || m_UseCount <= 1)
    {
        return;
    }

    iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_UseCount > 4 && m_Division >= 2)
    {
        RenderDivision();
    }
    else
    {
        RenderUsual();
    }

    iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void LocusHDR::RenderDivision()
{
    const float div = 1.0f / (float)m_Division;          //�P / �Z���̕�����
    const float PolygonT = 1.0f / ((float)(m_UseCount - 1)*(float)m_Division);  // ���������̂s�t�l�̑������
    const float fEp = 1 + FLT_EPSILON; // 1+�덷
    const float PointT = 1.0f / (float)(m_UseCount - 1);

    Point point[2]; // �O����P�ɕ`��
    Param param[2]; // �O����P�ɕ`��

    float d, t = 0, pret = 0;


    param[0] = m_StartParam; //�p�����[�^���X�^�[�g�ʒu�ɏ�����

    //�X�e�b�v�P�@�X�^�[�g���玟�̈ʒu�܂ł̃|���S��(��Ԍv�Z�ł��Ȃ�)
    {
        //1�n�_���̃p�����[�^���Ԃ��Aparam[1]�Ɋi�[
        LeapParam(param[1], m_StartParam, m_EndParam, PointT);

        //�`��
        RenderHDR(
            param[0],
            param[1],
            m_pPointData[0],
            m_pPointData[1],
            0,
            PointT
            );
    }


    //�X�e�b�v�Q�@�Q�߂���ŏI�̂ЂƂO�܂ł̃|���S�����Ԃ��Ȃ���`��
    {
        //�P�n�_���i�߂�
        t = PointT;
        LeapParam(param[0], m_StartParam, m_EndParam, PointT);

        for (size_t i = 0; i < m_UseCount - 3; ++i)
        {
            point[0] = m_pPointData[i + 1];

            for (d = div; d <= fEp; d += div)
            {
                pret = t;
                t += PolygonT;

                //�p�����[�^���Ԃ��Aparam[1]�Ɋi�[
                LeapParam(param[1], m_StartParam, m_EndParam, t);

                //���
                D3DXVec3CatmullRom(
                    (D3DXVECTOR3*)&point[1].pos, //out

                    (D3DXVECTOR3*)&m_pPointData[i].pos,
                    (D3DXVECTOR3*)&m_pPointData[i + 1].pos,
                    (D3DXVECTOR3*)&m_pPointData[i + 2].pos,
                    (D3DXVECTOR3*)&m_pPointData[i + 3].pos,

                    d
                    );

                //���
                D3DXVec3CatmullRom(
                    (D3DXVECTOR3*)&point[1].vec, //out

                    (D3DXVECTOR3*)&m_pPointData[i].vec,
                    (D3DXVECTOR3*)&m_pPointData[i + 1].vec,
                    (D3DXVECTOR3*)&m_pPointData[i + 2].vec,
                    (D3DXVECTOR3*)&m_pPointData[i + 3].vec,

                    d
                    );

                point[1].vec.Normalize();

                RenderHDR(
                    param[0],
                    param[1],
                    point[0],
                    point[1],
                    t,
                    pret
                    );

                point[0] = point[1];
                param[0] = param[1];
            }
        }
    }

    //�X�e�b�v�R�@�Ō�̈�O����Ō�̏ꏊ�Ɍ����Ẵ|���S��(��Ԍv�Z�ł��Ȃ�)
    {
        //�Ō�̂ЂƂO�̃p�����[�^���Z�o���Aparam[0]�Ɋi�[
        LeapParam(param[0], m_StartParam, m_EndParam, 1 - PointT);

        //�`��
        RenderHDR(
            param[0],
            m_EndParam,
            m_pPointData[m_UseCount - 2],
            m_pPointData[m_UseCount - 1],
            1 - PointT,
            1
            );
    }

}


void LocusHDR::RenderUsual()
{
    const float tAdd = 1 / (float)(m_UseCount - 1);

    Param param[2];
    float t = 0, pret = 0;;

    param[0] = m_StartParam;

    for (size_t i = 0; i < m_UseCount - 1; ++i)
    {
        pret = t;
        t += tAdd;

        LeapParam(param[1], m_StartParam, m_EndParam, t);

        RenderHDR(
            param[0],
            param[1],
            m_pPointData[i],
            m_pPointData[i + 1],
            t,
            pret
            );

        param[0] = param[1];
    }
}

void LocusHDR::RenderHDR(
    const Param& param1,
    const Param& param2,
    const Point& point1,
    const Point& point2,
    float t,
    float t2
    )
{

    static HdrLVERTEX v[4];

    v[0].pos = point1.pos - point1.vec*param1.Width;
    v[1].pos = point1.pos + point1.vec*param1.Width;

    v[2].pos = point2.pos - point2.vec*param2.Width;
    v[3].pos = point2.pos + point2.vec*param2.Width;

    v[0].tu = t;
    v[0].tv = 1;

    v[1].tu = t;
    v[1].tv = 0;

    v[2].tu = t2;
    v[2].tv = 1;

    v[3].tu = t2;
    v[3].tv = 0;


    v[0].color = v[1].color =
        D3DCOLOR_COLORVALUE(
        param1.Color.x,
        param1.Color.y,
        param1.Color.z,
        param1.Color.w
        );

    v[2].color = v[3].color =
        D3DCOLOR_COLORVALUE(
        param2.Color.x,
        param2.Color.y,
        param2.Color.z,
        param2.Color.w
        );


    v[0].HdrColor = v[1].HdrColor =
        D3DCOLOR_COLORVALUE(
        param1.HDRColor.x,
        param1.HDRColor.y,
        param1.HDRColor.z,
        param1.HDRColor.w
        );

    v[2].HdrColor = v[3].HdrColor =
        D3DCOLOR_COLORVALUE(
        param2.HDRColor.x,
        param2.HDRColor.y,
        param2.HDRColor.z,
        param2.HDRColor.w
        );


    iexPolygon::PolygonRender3DHDR(
        v, 2, m_pTexture, shader, "HDR_ForwardPolygon"
        );

}

void LocusHDR::LeapParam(
    Param& out,
    const Param& in1,
    const Param& in2,
    float t
    )
{
    out.Color = in1.Color*(1 - t) + in2.Color*t;
    out.HDRColor = in1.HDRColor*(1 - t) + in2.HDRColor*t;
    out.Width = in1.Width*(1 - t) + in2.Width*t;
}



