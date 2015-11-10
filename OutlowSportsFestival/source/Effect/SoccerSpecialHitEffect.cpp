#include "SoccerSpecialHitEffect.h"
#include "../GameSystem/ResourceManager.h"
#include "ParticleRenderer.h"
#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "HitEffectObject.h"
#include "AnimationBordRenderer.h"
#include "EffectFactory.h"
#include "GlavityLocus.h"

SoccerSpecialHit::SoccerSpecialHit(
    CharacterBase* pOwner,//�e�L�����N�^�ւ̃|�C���^
    CrVector3      pos,   //�o�����W
    CrVector3      vec,   //����
    RATIO          level  //�G�t�F�N�g�̃N�I���e�B(�O�`�P)
    ) :
    m_pOwner(pOwner),
    m_Pos(pos),
    m_Vec(vec),
    m_Level(level),
    m_Count(0),
    m_pStateFunc(&SoccerSpecialHit::State_Init)
{
    //���u���[off
    m_BlurSphere.Enable = false;
}

SoccerSpecialHit::~SoccerSpecialHit()
{

}

bool SoccerSpecialHit::Update()
{
    //���݂̃X�e�[�g�����s
    (this->*m_pStateFunc)();

    //�I���X�e�[�g�łȂ����true��Ԃ�
    return m_pStateFunc != &SoccerSpecialHit::State_Finish;
}

bool SoccerSpecialHit::Msg(MsgType mt)
{
    //���ɂ��邱�ƂȂ�
    return false;
}

void SoccerSpecialHit::EffectApper(int n)
{
    for (int i = 0; i < n; ++i)
    {
        Vector3 tvec(Vector3Normalize(Vector3Rand()));

        AnimationBordRenderer* r = new AnimationBordRenderer(
            DefResource.Get(Resource::TextureType::Anime_Circle),
            4,
            4,
            16,
            0x80FFFFFF,
            0x80FFFFFF
            );

        r->m_Pos = m_Pos;
        r->m_CellCount = 0;
        r->m_Size = Vector2(2, 2);


        Vector3Cross(r->m_Right, tvec, Vector3AxisX);

        if (r->m_Right == Vector3Zero)
        {
            Vector3Cross(r->m_Right, tvec, Vector3AxisZ);
        }

        Vector3Cross(r->m_Up, r->m_Right, tvec);


        r->m_Right.Normalize();
        r->m_Up.Normalize();

        AnimationBordGameObj* m = new AnimationBordGameObj(
            r
            );

        m->animation_end_delete = true;
        m->animation_loop = false;
        m->animation_speed = 1.0f;

        m->move_power = Vector3Zero;
        m->move_speed = Vector3Zero;

        m->scale_speed = Vector2(1, 1)*1.2f;
    }
}

void SoccerSpecialHit::Particle(int n)
{
    Vector3 power(0, -0.02f, 0);
    COLORf Color(0.1f, 1, 1, 1), HdrColor(0.5f, 1, 1, 1);

    for (int i = 0; i < n; ++i)
    {
        EffectFactory::LocusParticle(
            m_Pos,
            Vector3Rand()*2.0f, 
            power,
            0.05f,
            4,
            Color,
            HdrColor,
            120,
            0.5f
            );
    }
}

//--------------------------State_Func-----------------------------//

void SoccerSpecialHit::State_Init()
{
    //�������X�e�[�g
    
    {
        m_BlurSphere.Enable = true;
        m_BlurSphere.m_Pos = m_Pos;
        m_BlurSphere.m_Power = 50.0f;
        m_BlurSphere.m_Size = 20.0f;
    }


    //�u���[�k���X�e�[�g��
    m_pStateFunc = &SoccerSpecialHit::State_BlurToSmal;
}

void SoccerSpecialHit::State_BlurToSmal()
{
    //�u���[�͈͏k��
    {
        const float smal_val = 0.85f;

        m_BlurSphere.m_Size *= smal_val;
    }


    //�u���[�T�C�Y�����ȉ��Ȃ�
    if (m_BlurSphere.m_Size < 1.0f)
    {
        //�u���[�g��X�e�[�g��
        m_pStateFunc = &SoccerSpecialHit::State_Impact;
        m_BlurSphere.m_Size = 20.0f;
        m_BlurSphere.m_Power = 150.0f;

        //���ʉ�
        //Sound::Play(


        //�G�t�F�N�g
        EffectApper(8);
        Particle(10);
    }
    
}

void SoccerSpecialHit::State_Impact()
{

    //�p���[����߂�
    {
        const float smal_val = 0.9f;

        m_BlurSphere.m_Power *= smal_val;
    }


    //�p���[�@���ȉ��ŏI��
    if (m_BlurSphere.m_Power < 1.f)
    {
        m_pStateFunc = &SoccerSpecialHit::State_Finish;
    }
}

void SoccerSpecialHit::State_Finish()
{
    m_BlurSphere.Enable = false;
}