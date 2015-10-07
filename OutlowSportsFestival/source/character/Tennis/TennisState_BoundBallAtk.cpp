#include "TennisState_BoundBallAtk.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../Render/MeshRenderer.h"
#include "../../Ball/UsualBall.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Camera/Camera.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

TennisState_BoundBallAtk::TennisState_BoundBallAtk(
    ControllClass* pControllClass   //�o�E���h�������R���g���[������N���X(�I������delete����)
    ):
    m_pControllClass(pControllClass),
    m_Timer(0)

{

}

TennisState_BoundBallAtk::~TennisState_BoundBallAtk()
{
    delete m_pControllClass;
}


void TennisState_BoundBallAtk::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(TennisPlayer::_mt_BoundSmash);

    chr_func::ResetMove(t);
}


void TennisState_BoundBallAtk::Execute(TennisPlayer* t)
{
    const int SmashFrame = 15;
    const int EndFrame = 25;

    //�J�E���g�X�V
    ++m_Timer;

    //���n�_�I�u�W�F�N�g�𐶐�
    if (m_Timer == 1)
    {

    }

    //�T�E���h
    if (m_Timer == SmashFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }

    //����
    if (m_Timer == SmashFrame)
    {
        Vector3 pos, move;

        pos = t->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0);

        chr_func::GetFront(t, &move);
        move *= 0.3f;
        move.y = 0.6f;

        new TennisBoundBall(
            pos,
            move,
            t
            );
    }

    //�I���Œʏ�X�e�[�g��
    if (m_Timer == EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //��{�I�ȍX�V
    {
        TennisHitEvent HitEvent(t);
        chr_func::UpdateAll(t, &HitEvent);

        //���f���X�V
        t->m_Renderer.Update(1);

        //�s��X�V
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

    }
}


void TennisState_BoundBallAtk::Exit(TennisPlayer* t)
{

}



//****************************************************
//	�e�j�X_�o�E���h����{�[���N���X
//****************************************************


TennisBoundBall::TennisBoundBall(
    CrVector3  pos,
    CrVector3  first_move,
    TennisPlayer* const pOwner
    ) :
    m_Locus(20),
    m_pRigidBody(nullptr),
    m_pOwnerTennis(pOwner)
{
    //�{�[����{�p�����[�^������
    m_Params.pos = pos;
    m_Params.move = first_move;
    m_Params.pParent = pOwner;
    m_Params.scale = 0.1f;
    m_Params.type = BallBase::Type::_CantCounter;

    //�����X�e�[�g��ݒ�
    SetState(&TennisBoundBall::StateFly);

    //���b�V��������
    {
        iexMesh* pBallMesh;

        UsualBall::GetBallMesh(pOwner->m_PlayerInfo.chr_type, &pBallMesh);

        m_pBallRenderer = new MeshRenderer(
            pBallMesh,
            false,
            MeshRenderer::RenderType::UseColor,
            nullptr
            );
    }

    {
        //�O�Ղ̐ݒ�
        m_Locus.m_StartParam.Width = 0.25f;
        m_Locus.m_EndParam.Width = 0.025f;

        UpdateLocusColor();
    }

    {
        //�_���[�W����̍X�V
        m_Damage.Value = 2.5f;
        m_Damage.type = DamageBase::Type::_WeekDamage;
        m_Damage.pParent = pOwner;
        m_Damage.m_Enable = false;
        m_Damage.pBall = this;
        m_Damage.m_Param.size = 1.0f;

        UpdateDamage();
    }
}

TennisBoundBall::~TennisBoundBall()
{
    delete m_pBallRenderer;
    m_pBallRenderer = nullptr;

    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
    m_pRigidBody = nullptr;
}



bool TennisBoundBall::Update()
{
    (this->*m_pStateFunc)();

    UpdateMesh();
    UpdateLocusColor();
    UpdateDamage();

    return m_pStateFunc != &TennisBoundBall::StateFinish;
}


bool TennisBoundBall::Msg(MsgType mt)
{
    return false;
}



void TennisBoundBall::StateFly()
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
        if (DefCollisionMgr.RayPick(
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
            if (out.y > BallBase::UsualBallShotY*1.5f ||
                Vector3Normalize(vec).y < 0.5f)
            {
                SetState(&TennisBoundBall::StateNoDamage);

                m_Params.move = Vector3Refrect(m_Params.move, vec) * 0.5f;
            }
            else
            {
                //����
                SetState(&TennisBoundBall::StateGroundTouch);
            }

            //���������̈ʒu�ɕ␳����
            m_Params.pos.y = out.y + m_Params.scale;
        }
    }


    //�����Ǘ�(�X�e�[�W�O�ɂ��ʂ����ꍇ�̉��}���u
    if (++m_Timer > 300)
    {
        SetState(&TennisBoundBall::StateFinish);
    }

    //�ړ��X�V
    m_Params.pos += m_Params.move;

    //�O�Ղ̓_��ǉ�
    AddLocusPoint();

    //���ł���Ԃ͔��ˎ�̃e�j�X�����e�𔭎˂ł��Ȃ��悤��
    m_pOwnerTennis->SetDontBoundBallAtkTimer();

}

void TennisBoundBall::StateGroundTouch()
{
    //�������v���C���[�Ɍ����ċA���Ă����悤�ɐݒ�
    m_Params.move = m_Params.pParent->m_Params.pos - m_Params.pos;
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
            0x80FFA080,
            true
            );
    }

    //�r�d
    Sound::Play(Sound::Swing2);

    //�����蔻���L����
    m_Damage.m_Enable = true;

    //�ړ��X�e�[�g�Ɉڍs
    SetState(&TennisBoundBall::StateMove);
}

void TennisBoundBall::StateMove()
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
        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Ball
            ) != nullptr)
        {
            //����
            SetState(&TennisBoundBall::StateNoDamage);

            //���������̈ʒu�ɕ␳����
            m_Params.pos.y = out.y + m_Params.scale*0.5f;
        }
    }

    //�O�Ղ̓_��ǉ�
    AddLocusPoint();
}

void TennisBoundBall::StateNoDamage()
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
        if (m_Locus.m_StartParam.Color.w < 0.1f)
        {
            m_Locus.m_Visible = false;
        }

        if (m_Locus.m_Visible)
        {
            //�O�Ղ̓_��ǉ�
            AddLocusPoint();
        }
    }

    //���Ԍo�߂Ńt�F�[�h�A�E�g
    if (++m_Timer > 60)
    {
        SetState(&TennisBoundBall::StateCreateFadeOutBall);
    }
}

void TennisBoundBall::StateFinish()
{

}

void TennisBoundBall::StateCreateFadeOutBall() 
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
    SetState(&TennisBoundBall::StateFinish);
}

void TennisBoundBall::CreateRigidBody()
{
    //���łɍ쐬�ς݂Ȃ�return
    if (m_pRigidBody != nullptr)
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

void TennisBoundBall::SetState(StateFunc pNextState)
{
    m_pStateFunc = pNextState;
    m_Timer = 0;
}

void TennisBoundBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateLocusColor();

    m_Damage.type = DamageBase::Type::_VanishDamage;
}


void TennisBoundBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);
}

void TennisBoundBall::UpdateMesh()
{
    //���b�V���X�V
    const float scale = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);
    Matrix m;
    
    D3DXMatrixScaling(&m, scale, scale, scale);
    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pBallRenderer->SetMatrix(m);
}

void TennisBoundBall::UpdateLocusColor()
{
    //�O�ՐF�X�V
    const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Vector4(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f,
        0.5f
        );

    m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}


void TennisBoundBall::UpdateDamage()     //�����蔻��̍X�V
{
    m_Damage.vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}