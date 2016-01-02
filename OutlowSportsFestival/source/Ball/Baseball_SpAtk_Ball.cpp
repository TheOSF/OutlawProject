#include "Baseball_SpAtk_Ball.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Stage/PhysicsMeshObject.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/ThunderEffect.h"
#include "../Sound/Sound.h"
#include "../Effect/ImpactLightObject.h"

Baseball_SpAtk_Ball::Baseball_SpAtk_Ball(
    BaseballPlayer*     pParent,
    Vector3             pos,
    Vector3             move,
    float				damage_val		//�_���[�W��
    ) :
    m_DeleteFrame(0),
    m_pParent(pParent),
    m_Pos(pos),
    m_Vec(move),
    m_Locus(20),
    m_HitWall(false)
{

    {
        //�_���[�W����̃p�����[�^����
        m_Damage.pParent = pParent;
        m_Damage.m_Param.width = 0.5f;	//�傫���̓{�[���ɂ���ĈقȂ�?
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = damage_val;
        m_Damage.m_Enable = true;
        m_Damage.m_Param.pos1 = pos;
        m_Damage.m_Param.pos2 = pos;

        UpdateDamageClass();
    }

    {
        //�O��
        m_Locus.m_StartParam.Width=0.25f;
        m_Locus.m_StartParam.Color = Vector4(0, 0.5f, 1.0f, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 1);

        m_Locus.m_EndParam.Width = 0.0f;
        m_Locus.m_EndParam.Color = Vector4(0, 0.5f, 1.0f, 0);
        m_Locus.m_EndParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 0);
    }

    m_PointLight.param.color = Vector3(0, 0.5f, 1)*2.0f;
    m_PointLight.param.size = 35.0f;

	//�J�����̂��
	DefCamera.SetShock(Vector2(0.4f, 0.4f), 30);

    //�r�d
    Sound::Play(Sound::Beam2);
}

Baseball_SpAtk_Ball::~Baseball_SpAtk_Ball()
{
	
}


bool Baseball_SpAtk_Ball::Update()
{
    
    if (m_HitWall)
    {
        ++m_DeleteFrame;

        m_PointLight.param.size *= 0.9f;

        if (m_PointLight.param.size < 1.0f)
        {
            m_PointLight.Visible = false;
        }
        
    }
    else
    {
        //�ǔ���`�F�b�N
        m_HitWall = UpdateWallCheck();

        //�ړ�
        m_Pos += m_Vec;

        //�@���G�t�F�N�g
        for (int i = 0; i < 3; i++)
        {
            new ThunderEffect
                (m_Pos - m_Vec*0.33f*(float)i,
                m_Pos + Vector3Rand() * 5,
                0.5f+frand(),
                0.03f,
                35,
                Vector4(0, 0, 1, 0),
                5);
        }

        //�_���[�W����X�V
        UpdateDamageClass();
    }

    //�O��
    {
        Vector3 vec;

        Vector3Cross(vec, DefCamera.GetForward(), m_Vec);
        vec.Normalize();

        m_Locus.AddPoint(
            m_Pos,
            vec
            );
    }

    {
        //�X�e�[�W�O
        if (m_Pos.Length() > 150.0f)
        {
            m_Damage.m_Enable = false;
            ++m_DeleteFrame;
        }
    }

    //���C�g�X�V
    m_PointLight.param.pos = m_Pos;

    return (m_DeleteFrame < 20);//�P�O�t���[���ŏ���
}


bool Baseball_SpAtk_Ball::Msg(MsgType mt)
{

	return false;
}


void Baseball_SpAtk_Ball::UpdateDamageClass()
{
    m_Damage.m_Vec = m_Vec;
    m_Damage.m_Vec.Normalize();
    m_Damage.m_Vec.y = 0.3f;

    m_Damage.m_VecPower.x = 0.5f;
    m_Damage.m_VecPower.y = 0.3f;

	m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1;
	m_Damage.m_Param.pos1 = m_Pos;
}


bool Baseball_SpAtk_Ball::UpdateWallCheck()
{
    Vector3 Out, Pos(m_Pos), Vec(Vector3Normalize(m_Vec));
    float Dist = m_Vec.Length()*1.1f;
	int Material;

	Vec.Normalize();

    if (DefCollisionMgr.RayPick(
        &Out,
        &Pos,
        &Vec,
        &Dist,
        &Material,
        CollisionManager::RayType::_Ball
        )
        )
    {

        m_Damage.m_Enable = false;

        m_Pos = Out - m_Vec;

        for (int i = 0; i < 10; ++i)
        {
            EffectFactory::SmokeParticle(
                Out + Vector3Rand()*0.1f, 
                Vector3(0, 0.05f, 0) + Vector3Rand()*0.01f,
                40,
                0.8f,
                0x10FFFFFF
                );
        }

        //EffectFactory::

        return true;
    }

    return false;
}



