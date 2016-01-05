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
    RATIO          level,  //�G�t�F�N�g�̃N�I���e�B(�O�`�P)
    UINT           time
    ) :
    m_pOwner(pOwner),
    m_Pos(pos),
    m_Level(level),
    m_Count(0),
    m_pStateFunc(&SoccerSpecialHit::State_Init),
    m_LiveTime((int)time)
{
    //�����̐�����уR���g���[���N���X�𐶐�����N���X
    class GetTornadoDamageControllTransformClass :public DamageControllVanish::GetDamageControllTransformClass
    {
    private:
        const Vector3 m_Pos;

    public:
        GetTornadoDamageControllTransformClass(CrVector3 Pos):
            m_Pos(Pos)
        {

        }

        DamageControll_Transform* Get()
        {


            DamageControll_Transform* dct = new DamageControll_Transform();


            {
                TornadoVanishControll::InitParam param;
                const float Frame = 50.0f;

                param.from = m_Pos;

                param.start_angle_seed = frand()*PI*2.0f;
                param.all_rotation_angle = PI * 6;

                param.start_width = 1.0f;
                param.end_width = 2.0f;

                param.height = 6.5f;
                param.speed = 1.0f / Frame;
                param.damage = 30.0f;
                

                TornadoVanishControll* tvc = new TornadoVanishControll(param, dct);
            }

            return dct;
        }

    };

    m_pDamage = new DamageControllVanish(new GetTornadoDamageControllTransformClass(pos));

    //�_���[�W�ݒ�
    m_pDamage->HitCount = 0;
    m_pDamage->m_Enable = true;
    m_pDamage->m_Param.pos = m_Pos;
    m_pDamage->m_Param.size = 3.5f;
    m_pDamage->pBall = nullptr;
    m_pDamage->pParent = pOwner;
    m_pDamage->type = DamageBase::Type::_ControllDamage;
    m_pDamage->Value = 2.5f*level;
    m_pDamage->MaxChrHit = 10;

    //����
    TornadoEffect::Param p;

    p.Length = 17.0f;
    p.LocusWidthEnd = 0.4f;
    p.LocusWidthStart = 1.5f;
    p.lowWidth = 0.1f;
    p.middleWidth = 1.5f;
    p.highWidth = 0.1f;
    p.pos = m_Pos + Vector3(0, -5, 0);
    p.right = Vector3AxisX;
    p.RotateSpeed = 0.8f;
    p.vec = Vector3AxisY;
    p.middle_height = 12;
    
    m_pTornadoEffect = new TornadoEffect(
        p,
        3,
        40
        );


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

void SoccerSpecialHit::EffectApper(int n, RATIO scale)
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

        r->m_Pos = m_Pos + Vector3(0, 3, 0);
        r->m_CellCount = 0;
        r->m_Size = Vector2(2, 2)*scale;


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

        m->scale_speed = Vector2(1, 1)*1.2f*scale;
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

    //�u���[�k���X�e�[�g��
    m_pStateFunc = &SoccerSpecialHit::State_BlurToSmal;
}

void SoccerSpecialHit::State_BlurToSmal()
{
    m_Count++;

    m_pDamage->m_Enable = m_Count < 10;


    //�p���[����߂�
    if (m_Count > m_LiveTime)
    {
        const float smal_val = 0.8f;

        m_pTornadoEffect->m_Param.LocusWidthStart *= smal_val;
        m_pTornadoEffect->m_Param.LocusWidthEnd *= smal_val;

        m_pTornadoEffect->m_Param.highWidth *= smal_val;
        m_pTornadoEffect->m_Param.middleWidth *= smal_val;
        m_pTornadoEffect->m_Param.lowWidth *= smal_val;
    }
    else
    {
        m_pTornadoEffect->m_Param.highWidth += (8.0f - m_pTornadoEffect->m_Param.highWidth)*0.07f;
        m_pTornadoEffect->m_Param.lowWidth += (4.0f - m_pTornadoEffect->m_Param.lowWidth)*0.07f;;
    }

    
    if (m_Count > m_LiveTime + 15)
    {
        //�I���X�e�[�g��
        m_pStateFunc = &SoccerSpecialHit::State_Finish;

        m_pTornadoEffect->Destroy(); //Gameobject��delete�ł��Ȃ�����
        
    }
}


void SoccerSpecialHit::State_Finish()
{

}



//-----------------------------------------------------//
//   �T�b�J�[�̕K�E�Z���q�b�g�����Ƃ�������ё���N���X
//-----------------------------------------------------//

TornadoVanishControll::TornadoVanishControll(
    const InitParam& param,
    DamageControll_Transform* pControll_Transform):

    m_Param(param),
    m_pControll_Transform(pControll_Transform),
    m_Count(0),
    m_DamageCount(0)
{
    //�����X�V
    SetTransform();
}

TornadoVanishControll::~TornadoVanishControll()
{

}


bool TornadoVanishControll::Update()
{
    bool live_flg = true;


    //�ړ��X�V
    live_flg = Move();

    //�p���s����X�V
    SetTransform();

    {
        //�_���[�W�����Z
        const int Frame = 6;

        if (++m_DamageCount % Frame == 0 || live_flg == false)
        {   
            AddDamage(m_Param.damage * m_Param.speed * float(Frame), live_flg);
        }
    }

    //�����t���[���Ȃ瑀�삵�Ă���N���X������
    if (live_flg == false)
    {
        m_pControll_Transform->m_Destroy = true;
    }

    return live_flg;
}

bool TornadoVanishControll::Msg(MsgType mt)
{


    return false;
}

//�ړ��X�V
bool TornadoVanishControll::Move()
{
    //���Ԍo��
    m_Count += m_Param.speed;
    

    //�I������
    if (m_Count > 1.0f)
    {
        m_Count = 1.0f;

        return false;
    }

    return true;
}


//�p���s����X�V
void TornadoVanishControll::SetTransform()
{
    Matrix* const pT = &m_pControll_Transform->m_Transform;
    Matrix R;

    //���݂̉�]�p�x
    const RADIAN RotateValue = (m_Param.all_rotation_angle * m_Count) + m_Param.start_angle_seed;

    //������
    D3DXMatrixIdentity(pT);

    //X��](�����̂ق��ɑ̂�������
    if(0)
    {
        D3DXMatrixRotationX(&R, D3DXToRadian(45));

        *pT *= R;
    }

    //Y��](�i�s�����Ɍ�����
    {
        D3DXMatrixRotationY(&R, RotateValue + D3DXToRadian(90));

        *pT *= R;
    }

    
    //�ʒu���Z�o
    {
        Vector3 Pos;

        const float width = m_Param.start_width + (m_Param.end_width - m_Param.start_width)*m_Count;

        Pos = m_Param.from + Vector3(sinf(RotateValue)*width, m_Param.height*m_Count, cosf(RotateValue)*width);

        //�ʒu���Z�b�g
        {
            pT->_41 = Pos.x;
            pT->_42 = Pos.y;
            pT->_43 = Pos.z;
        }
    }

}


//�_���[�W�����Z
void TornadoVanishControll::AddDamage(float value,bool dontDie)
{
    //�_���[�W��^����
    m_pControll_Transform->AddDamage(value, dontDie);
    
}

