#include"ParticleManager.h"
#include"../Camera/Camera.h"
#include "../utillity/ColorUtility.h"


ParticleManager* ParticleManager::m_pInstance = nullptr;

void ParticleManager::Initialze(
    UINT         maxParticle,  //�p�[�e�B�N���ő吔
    iex2DObj*    pTexture,     //�p�[�e�B�N���e�N�X�`��
    UINT         TextureCellX, //�e�N�X�`���R�}���w
    UINT         TextureCellY  //�e�N�X�`���R�}���w
    )
{
    MyAssert(m_pInstance == nullptr, "�C���X�^���X�̓�d���������s���܂���");

    m_pInstance = new ParticleManager(
        maxParticle,
        pTexture,
        TextureCellX,
        TextureCellY
        );
}

ParticleManager& ParticleManager::GetInstance()
{
    MyAssert(m_pInstance, "�C���X�^���X���Ȃ���Ԃ�ParticleManager::GetInstance���Ă΂�܂���");

    return *m_pInstance;
}

void ParticleManager::Release()
{
    MyAssert(m_pInstance, "�C���X�^���X���Ȃ���Ԃ�ParticleManager::Release���Ă΂�܂���");

    delete m_pInstance;
    m_pInstance = nullptr;
}

//�p�[�e�B�N���Z�b�g
void ParticleManager::Set(
    UINT        type,
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,

    UINT        md_frame,
    UINT        end_frame,

    DWORD       start_color,
    DWORD       md_color,
    DWORD       end_color,
    DWORD       dw_flag      //�`��t���O
    )
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[m_SetCount];

        if (it->count >= it->end_frame)
        {
            m_SetCount = (m_SetCount + 1) % iMax;
            continue;
        }

        it->type = type;
        it->pos = pos;
        it->move = move;
        it->power=power;
        it->size = size;

        it->md_frame = md_frame;
        it->end_frame = end_frame;

        it->start_color = start_color;
        it->md_color = md_color;
        it->end_color = end_color;

        it->dw_flag = dw_flag;

        return;
    }

    MyAssert(false, "�p�[�e�B�N�����o���ő吔�ɒB���܂��� ���݂̐� = %d ", iMax);
}


//�p�[�e�B�N���Z�b�g
void ParticleManager::Set(
    UINT        type,
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,
    UINT        end_frame,
    DWORD       color,
    DWORD       dw_flag
    )
{
    Set(
        type,
        pos,
        move,
        power,
        size,
        end_frame / 2,
        end_frame,
        color,
        color,
        color,
        dw_flag
        );
}


//�`��
void ParticleManager::Render()
{
    
    ParticleData* it;

    const Vector3 R = DefCamera.GetRight();
    const Vector3 U = DefCamera.GetUp();
    
    LVERTEX v[4];

    const int iMax = (int)m_MaxParticle;
    RATIO t;

    const Vector2 TexCell(1.0f / (float)m_TextureCellX, 1.0f / (float)m_TextureCellY);

    //�p�[�e�B�N�����\�[�g
    ParticleSort();


    for (int i = 0; i < iMax; ++i)
    {
        it = m_pWorkParticleDataPointerArray[i];

        //�����m�F
        if (it->count >= it->end_frame)
        {
            continue;
        }

        {
            //�ꏊ�Z�b�g
            v[0].pos = R * -it->size.x + U*it->size.y + it->pos;
            v[1].pos = R *  it->size.x + U*it->size.y + it->pos;
            v[2].pos = R * -it->size.x + -U*it->size.y + it->pos;
            v[3].pos = R *  it->size.x + -U*it->size.y + it->pos;
        }

        {
            //�F�̕��
            if (it->count < it->md_frame)
            {
                t = (float)it->count / (float)it->md_frame;

                v[0].color = ColorLerp(it->start_color, it->md_color, t);
            }
            else
            {
                t = (float)(it->count - it->md_frame) / (float)(it->end_frame - it->md_frame);

                v[0].color = ColorLerp(it->md_color, it->end_color, t);
            }

            //�e���_�ɃZ�b�g
            v[1].color = v[2].color = v[3].color = v[0].color;
        }


        //�e�N�X�`�����W���v�Z
        {
            //����
            v[0].tu = (float)(it->count%m_TextureCellX)*TexCell.x;
            v[0].tv = (float)(it->count/m_TextureCellX)*TexCell.y;

            //���̑�
            v[1].tu = v[0].tu + TexCell.x;
            v[1].tv = v[0].tv;

            v[2].tu = v[0].tu;
            v[2].tv = v[0].tv + TexCell.y;

            v[3].tu = v[1].tu;
            v[3].tv = v[2].tv;
        }

        iexPolygon::Render3D(
            v,
            2,
            m_pTexture,
            it->dw_flag
            );
    }
}


//�X�V
void ParticleManager::Update()
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[i];

        if (it->count >= it->end_frame)
        {
            continue;
        }

        it->count += 1;
        it->move += it->power;
        it->pos += it->move;
    }
}



ParticleManager::ParticleManager(
    UINT         maxParticle,  //�p�[�e�B�N���ő吔
    iex2DObj*    pTexture,     //�p�[�e�B�N���e�N�X�`��
    UINT         TextureCellX, //�e�N�X�`���R�}���w
    UINT         TextureCellY  //�e�N�X�`���R�}���w
    ) :
    m_MaxParticle(maxParticle),
    m_pTexture(pTexture),
    m_TextureCellX(TextureCellX),
    m_TextureCellY(TextureCellY),
    m_SetCount(0)
{
    m_ParticleDataArray = new ParticleData[maxParticle];
    m_pWorkParticleDataPointerArray = new ParticleData*[maxParticle];
}


ParticleManager::~ParticleManager()
{
    delete[] m_ParticleDataArray;
    delete[] m_pWorkParticleDataPointerArray;
}

//Z�l��r�p�֐�
static int CompareParticleZFunc(const void*p1, const void* p2)
{
    return ((*(ParticleManager::ParticleData*)p1).cameraZ > (*(ParticleManager::ParticleData*)p2).cameraZ) ? (-1) : (1);
}


void ParticleManager::ParticleSort()
{
    const int iMax = (int)m_MaxParticle;
    ParticleData* it;

    for (int i = 0; i < iMax; ++i)
    {
        it = &m_ParticleDataArray[i];

        //Z�l���v�Z����
        it->cameraZ = DefCamera.GetCameraZ(it->pos);


        m_pWorkParticleDataPointerArray[i] = &m_ParticleDataArray[i];
    }

    //�������ԂɃ\�[�g
    std::qsort(
        &m_pWorkParticleDataPointerArray[0],
        m_MaxParticle,
        sizeof(ParticleData*),
        CompareParticleZFunc
        );
}