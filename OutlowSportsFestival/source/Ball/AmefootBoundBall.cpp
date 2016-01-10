#include "AmefootBoundBall.h"
#include "../character/CharacterFunction.h"
#include "../character/Amefoot/AmefootUsualHitEvent.h"
#include "../character/Amefoot/AmefootPlayerState.h"
#include "../Render/MeshRenderer.h"
#include "UsualBall.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/HitEffectObject.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "../character/CharacterManager.h"

// �A���t�g�̃o�E���h����{�[���N���X

AmefootBoundBall::AmefootBoundBall(
    CrVector3  pos,
    CrVector3  first_move,
    AmefootPlayer* const pOwner
    ) :
    m_Locus(15),
    m_pRigidBody(nullptr),
    m_pOwner(pOwner)
{
    //�{�[����{�p�����[�^������
    m_Params.pos = pos;
    m_Params.move = first_move;
    m_Params.pParent = pOwner;
    m_Params.scale = 0.3f;
    m_Params.type = BallBase::Type::_CantCounter;

    //�����X�e�[�g��ݒ�
    SetState(&AmefootBoundBall::StateFly);

    //���b�V��������
    {
        iexMesh* pBallMesh;

        UsualBall::GetBallMesh(pOwner->m_PlayerInfo.chr_type, &pBallMesh);

        m_pBallRenderer = new MeshRenderer(
            pBallMesh,
            false,
            MeshRenderer::RenderType::UseColor
            );

        UpdateMesh();
    }

    {
        //�O�Ղ̐ݒ�
        m_Locus.m_StartParam.Width = 0.36f;
        m_Locus.m_EndParam.Width = 0.1f;

        UpdateLocusColor();
    }

    {
        //�_���[�W����̍X�V
        m_Damage.Value = 2.5f;
        m_Damage.type = DamageBase::Type::_WeekDamage;
        m_Damage.pParent = pOwner;
        m_Damage.m_Enable = true;
        m_Damage.pBall = this;
        m_Damage.m_Param.size = 1.0f;
        m_Damage.MaxChrHit = 1;
        m_Damage.HitMotionFrame = 35;

        UpdateDamage();
    }

    m_BallEffect.SetParent(this);
}


AmefootBoundBall::~AmefootBoundBall()
{
    delete m_pBallRenderer;
    m_pBallRenderer = nullptr;

    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
    m_pRigidBody = nullptr;
}


bool AmefootBoundBall::Update()
{
    (this->*m_pStateFunc)();

    UpdateMesh();
    UpdateLocusColor();
    UpdateDamage();
    m_BallEffect.Update();

    return m_pStateFunc != &AmefootBoundBall::StateFinish;
}


bool AmefootBoundBall::Msg(MsgType mt)
{
    return false;
}



void AmefootBoundBall::StateFly()
{
    const float Glavity = -0.02f;

    //�d�͌v�Z
    m_Params.move.y += Glavity;

    //�X�e�[�W�Ƃ̔���
    {
        Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
        float dist = m_Params.move.Length()*1.5f;
        int material = 0;

        //�X�e�[�W�Ƃ������Ă�����
        if ( DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Ball
            ) != nullptr
            )
        {
            //�������Ă����ꏊ�� �������� or ���łȂ��@�ꍇ�͎��s
            if ( out.y > BallBase::UsualBallShotY*1.5f ||
                Vector3Normalize(vec).y < 0.5f )
            {
                SetState(&AmefootBoundBall::StateNoDamage);

                m_Params.move = Vector3Refrect(m_Params.move, vec) * 0.5f;
            }
            else
            {
                //����
                SetState(&AmefootBoundBall::StateGroundTouch);
            }

            //���������̈ʒu�ɕ␳����
            m_Params.pos.y = out.y + m_Params.scale;
            return;
        }
    }


    //�����Ǘ�(�X�e�[�W�O�ɂ��ʂ����ꍇ�̉��}���u
    if ( ++m_Timer > 300 )
    {
        SetState(&AmefootBoundBall::StateFinish);
    }

    //�ړ��X�V
    m_Params.pos += m_Params.move;

    //�O�Ղ̓_��ǉ�
    AddLocusPoint();

    //�G�t�F�N�g�X�V
    m_BallEffect.Update();
}

void AmefootBoundBall::StateGroundTouch()
{
    //�O�Ղ̓_��ǉ�
    AddLocusPoint();

    m_Params.move = Vector3Zero;

    //�����蔻���L����
    m_Damage.m_Enable = true;

    //if ( ++m_Timer > 5 )
    {

        //��ԋ߂��v���C���[�Ɍ������Ă����悤�ɐݒ�
        const auto& chr_map = DefCharacterMgr.GetCharacterMap();
        float dist = FLT_MAX;
        CharacterBase* pTarget = m_pOwner;
        for ( auto&& chr : chr_map )
        {
            float temp = (chr.first->m_Params.pos - m_Params.pos).Length();

            if ( dist < temp )continue;

            dist = temp;
            pTarget = chr.first;
        }

        m_Params.move = pTarget->m_Params.pos - m_Params.pos;
        m_Params.move.y = 0;

        m_Params.move.Normalize();
        m_Params.move *= 0.55f;

        m_Params.type = BallBase::Type::_Usual;

        //�G�t�F�N�g
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number));

            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                m_Params.pos,
                Vector3Normalize(m_Params.move),
                0.1f,
                0.1f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
                );

            //�p�[�e�B�N��
            EffectFactory::Smoke(
                m_Params.pos,
                Vector3Zero,
                2.0f,
                1.0f,
                true
                );
        }

        {
            //���b�V�������点��
            COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

            m_pBallRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
            m_pBallRenderer->m_Lighting = Vector3(1, 1, 1) * 0.1f;
        }

        //�r�d
        Sound::Play(Sound::Swing2);

        //�ړ��X�e�[�g�Ɉڍs
        SetState(&AmefootBoundBall::StateMove);

    }
}

void AmefootBoundBall::StateMove()
{
    //���ɂ��Ă���̂ŁA�K�؂ȍ��� ( BallBase::UsualBallShotY ) �܂ŏ㏸����
    m_Params.pos.y += (BallBase::UsualBallShotY - m_Params.pos.y) * 0.2f;

    //�ړ�
    m_Params.pos += m_Params.move;

    //�X�e�[�W�Ƃ̔���
    {
        Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
        float dist = m_Params.move.Length() * 1.5f;
        int material = 0;

        //�X�e�[�W�Ƃ������Ă�����
        if ( DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Ball
            ) != nullptr )
        {
            //����
            SetState(&AmefootBoundBall::StateNoDamage);

            //���������̈ʒu�ɕ␳����
            m_Params.pos.y = out.y + m_Params.scale*0.5f;
        }
    }

    //�����Ǘ�(�X�e�[�W�O�ɂ��ʂ����ꍇ�̉��}���u
    if ( ++m_Timer > 300 )
    {
        SetState(&AmefootBoundBall::StateFinish);
    }

    //�O�Ղ̓_��ǉ�
    AddLocusPoint();
}

void AmefootBoundBall::StateNoDamage()
{
    //���̍쐬
    CreateRigidBody();

    //�����蔻��𖳌���
    m_Damage.m_Enable = false;

    //�{�[���^�C�v���J�E���^�[�ł��Ȃ��^�C�v�ɐݒ�
    m_Params.type = BallBase::Type::_DontWork;

    //RigidBody�N���X�̍s������g�ɓK�p����
    {
        Matrix M;
        Vector3 PrePos = m_Params.pos;

        m_pRigidBody->Get_TransMatrix(M);

        M = m_BaseMatrix * M;

        m_pBallRenderer->SetMatrix(M);

        m_Params.pos = Vector3(M._41, M._42, M._43);
        m_Params.move = m_Params.pos - PrePos;

    }

    //�O�Ղ̑��������X�Ɍ��炵�Ă���
    {
        //�O��
        m_Locus.m_StartParam.Color.w *= 0.95f;

        //���������ȉ��Ȃ�`�悵�Ȃ�
        if ( m_Locus.m_StartParam.Color.w < 0.1f )
        {
            m_Locus.m_Visible = false;
        }

        if ( m_Locus.m_Visible )
        {
            //�O�Ղ̓_��ǉ�
            AddLocusPoint();
        }
    }

    //���C�g����
    {
        m_pBallRenderer->m_HDR *= 0.1f;
        m_pBallRenderer->m_Lighting *= 0.1f;
    }

    //���Ԍo�߂Ńt�F�[�h�A�E�g
    if ( ++m_Timer > 60 )
    {
        SetState(&AmefootBoundBall::StateCreateFadeOutBall);
    }
}

void AmefootBoundBall::StateFinish()
{

}

void AmefootBoundBall::StateCreateFadeOutBall()
{
    //�t�F�[�h�A�E�g���ď�����{�[���N���X���쐬����
    iexMesh* pMesh;

    //���̍쐬
    CreateRigidBody();

    UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

    new BallFadeOutRenderer(
        pMesh,
        m_BaseMatrix,
        m_pRigidBody,
        60
        );

    //���g�ŊJ�����Ȃ��悤��null�ɐݒ肵�Ă���
    m_pRigidBody = nullptr;


    //�X�e�[�g���I���ɐݒ�
    SetState(&AmefootBoundBall::StateFinish);
}

void AmefootBoundBall::CreateRigidBody()
{
    //���łɍ쐬�ς݂Ȃ�return
    if ( m_pRigidBody != nullptr )
    {
        return;
    }

    m_BaseMatrix = m_pBallRenderer->GetMatrix();

    m_BaseMatrix._41 = 0;
    m_BaseMatrix._42 = 0;
    m_BaseMatrix._43 = 0;

    const UsualBall::PhysicsParam p = UsualBall::GetBallPhysics(m_Params.pParent->m_PlayerInfo.chr_type);


    m_pRigidBody = DefBulletSystem.AddRigidSphere(
        p.Mass,
        RigidBody::ct_dynamic,
        m_Params.pos,
        Vector3Zero,
        p.Radius,
        p.Friction,
        p.Restitution,
        m_Params.move * 45.0f
        );
}

void AmefootBoundBall::SetState(StateFunc pNextState)
{
    m_pStateFunc = pNextState;
    m_Timer = 0;
}

void AmefootBoundBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateLocusColor();
    m_BallEffect.Counter();

    m_Damage.ResetCounts();
    m_Damage.type = DamageBase::Type::_VanishDamage;
}


void AmefootBoundBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);
}

void AmefootBoundBall::UpdateMesh()
{
    //���b�V���X�V
    const float scale = UsualBall::GetBallScale(CharacterType::_Americanfootball);
    Matrix m;

    D3DXMatrixScaling(&m, scale, scale, scale);
    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pBallRenderer->SetMatrix(m);
}

void AmefootBoundBall::UpdateLocusColor()
{
    //�O�ՐF�X�V
    const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Color.toVector4();
    m_Locus.m_StartParam.Color.w = 0.3f;
    m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;

    m_Locus.m_StartParam.HDRColor.w = 0.5f;

    m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
    m_Locus.m_EndParam.Color.w = 0;

    m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
    m_Locus.m_EndParam.HDRColor.w = 0;


}


void AmefootBoundBall::UpdateDamage()     //�����蔻��̍X�V
{
    m_Damage.m_Vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}



