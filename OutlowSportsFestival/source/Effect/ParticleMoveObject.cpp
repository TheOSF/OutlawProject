#include "ParticleMoveObject.h"

ParticleMoveObject::ParticleMoveObject(
    ParticleBase* pParticle,
    CrVector3     move,
    CrVector3     power,
    UINT          live_time,
    bool          animation,
    UINT          cellX,
    UINT          cellY
    ) :
    pParticle(pParticle),
    move(move),
    power(power),
    delete_time(live_time),
    time(0),
    start_color(pParticle->m_Param.color),
    animation(animation),
    cellX(cellX),
    cellY(cellY)
{
    //��x�X�V
    Update();

    //���������P�t���[���߂�
    --time;
}


ParticleMoveObject::~ParticleMoveObject()
{
    delete pParticle;
}


bool ParticleMoveObject::Update()
{
    //�ړ�
    pParticle->m_Param.pos += move;
    move += power;

    //�F�̕��
    {
        COLORf end_color = start_color;
        end_color.a = 0;
        pParticle->m_Param.color = COLORf::Lerp(start_color, end_color, (float)time / (float)delete_time);
    }
    //�e�N�X�`���A�j���[�V����
    if (animation)
    {
        pParticle->SetCellUV((int)cellX, (int)cellY, (int)time);
    }

    //�����J�E���gor�t�B�[���h���痣�ꂷ���Ă���Ə�����
    return
        delete_time > ++time &&
        pParticle->m_Param.pos.Length() < 150;
}


bool ParticleMoveObject::Msg(MsgType mt)
{
    return false;
}



//-----------------------------------------------------------
//      �F�̊Ǘ����ł���ver
//-----------------------------------------------------------

ParticleMoveObject2::ParticleMoveObject2(
    ParticleBase* pParticle,   //�������p�[�e�B�N���ւ̃|�C���^
    CrVector3     move,        //�ړ���
    CrVector3     power,       //�͏�(�d�͂Ƃ�)
    bool          animation,   //�e�N�X�`���A�j���[�V���������邩�ǂ���
    UINT          cellX,       //�e�N�X�`���̃R�}����(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)
    UINT          cellY,        //�e�N�X�`���̃R�}���c(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)

    UINT          md_frame,
    UINT          end_frame,

    DWORD         md_color,
    DWORD         end_color
    ):
    pParticle(pParticle),
    move(move),
    power(power),

    time(0),

    animation(animation),
    cellX(cellX),
    cellY(cellY),

    start_color(pParticle->m_Param.color.toDWORD()),
    md_color(md_color),
    end_color(end_color),

    md_frame(md_frame),
    end_frame(end_frame)
{
        //��x�X�V
        Update();

        //���������P�t���[���߂�
        --time;
    }

ParticleMoveObject2::~ParticleMoveObject2()
{
    delete pParticle;
}


bool ParticleMoveObject2::Update()
{
    //�ړ�
    pParticle->m_Param.pos += move;
    move += power;

    //�F�̕��
    {
        float t;
        if (time < md_frame)
        {
            t = (float)time / (float)md_frame;

            pParticle->m_Param.color = COLORf(ColorLerp(start_color, md_color, t));
        }
        else
        {
            t = (float)(time - md_frame) / (float)(end_frame - md_frame);

            pParticle->m_Param.color = COLORf(ColorLerp(md_color, end_color, t));
        }
        
    }
    //�e�N�X�`���A�j���[�V����
    if (animation)
    {
        pParticle->SetCellUV((int)cellX, (int)cellY, (int)time);
    }

    //�����J�E���gor�t�B�[���h���痣�ꂷ���Ă���Ə�����
    return
        end_frame > ++time &&
        pParticle->m_Param.pos.Length() < 150;
}


bool ParticleMoveObject2::Msg(MsgType mt)
{
    return false;
}


