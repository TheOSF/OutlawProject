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

    //����
    if (m_Timer == SmashFrame)
    {
        BallBase::Params param;

        param.pos = t->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0);
        param.pParent = t;
        param.type = BallBase::Type::_CantCounter;


        chr_func::GetFront(t, &param.move);
        param.move *= 0.5f;
        param.move.y = 0.8f;

        new TennisBoundBall(
            param,
            -0.04f,
            Vector3AxisX
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
    BallBase::Params	params,			//�{�[���p�����[�^
    float               glavity,        //�d��
    CrVector3           AtkVec          //�o�E���h�������
    ) :
    m_Locus(8),
    m_pStateFunc(&TennisBoundBall::State_Flying),
    m_Glavity(glavity),
    m_Timer(0),
    m_MoveVec(AtkVec)
{
    m_Params = params;

    //���b�V���쐬
    {
        iexMesh* pMesh = nullptr;

        UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pMesh);

        m_pMeshRenderer = new MeshRenderer(pMesh, false, MeshRenderer::RenderType::UseColor);
    }

    {
        //���[���h�ϊ��s��̍X�V
        D3DXQuaternionIdentity(&m_BallRot);
        UpdateTransMatrix(0.5f);
    }

    {
        //�O��
        m_Locus.m_StartParam.Width = 0.25f;
        m_Locus.m_EndParam.Width = 0.05f;

        m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
    }
}

TennisBoundBall::~TennisBoundBall()
{
    delete m_pMeshRenderer;
}

bool TennisBoundBall::Update()
{
    {
        Vector3 vec;

        Vector3Cross(vec, DefCamera.GetForward(), m_Params.move);
        vec.Normalize();

        m_Locus.AddPoint(m_Params.pos, vec);
    }


    (this->*m_pStateFunc)();

    
    
    return m_pStateFunc != &TennisBoundBall::State_Finish;
}

bool TennisBoundBall::Msg(MsgType mt)
{
    return false;
}

void TennisBoundBall::UpdateTransMatrix(RATIO scaleYratio)
{
    Matrix M;

    GetTransMatrix(M, scaleYratio);

    m_pMeshRenderer->SetMatrix(M);
}

//���[���h�ϊ��s����Z�o����
void TennisBoundBall::GetTransMatrix(Matrix& m,RATIO scaleYratio)
{
    Matrix T;

    {
        const float scale = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);
        D3DXMatrixScaling(&m, scale, scale*scaleYratio, scale);
    }

    {
        D3DXMatrixRotationQuaternion(&T, &m_BallRot);
        m *= T;
    }

    {
        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;
    }

}


void TennisBoundBall::RotateY()
{
    //�x����]��I
    {
        D3DXQUATERNION q;

        D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(10), 0, 0);

        m_BallRot *= q;
    }
}



void TennisBoundBall::State_Flying()
{
    m_Params.move.y += m_Glavity;
    m_Params.pos += m_Params.move;

    RotateY();
    UpdateTransMatrix(0.5f);

    {
        Vector3 out, pos(m_Params.pos), vec(m_Params.move);
        float dist = m_Params.move.Length()*1.5f;
        int material;

        vec.Normalize();

        if (
            DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Usual
            ) != nullptr)
        {
            //���������ʂ���̏�����ŁA���Ⴓ���Ȃ��ꍇ����
            if (
                Vector3Radian(Vector3AxisY, vec) < D3DXToRadian(45) &&
                BallBase::UsualBallShotY > out.y
                )
            {
                m_Params.pos = out + Vector3(0, 0.5f, 0);
                m_pStateFunc = &TennisBoundBall::State_Rolling;
            }
            else
            {
                //���s�̏ꍇ(�ǂɓ�������)
                m_pStateFunc = &TennisBoundBall::State_Failed;
            }
        }
    }
}

void TennisBoundBall::State_Failed()
{
    iexMesh*   pMesh;
    Matrix     BaseMatrix;
    RigidBody* pRigidBody;

    UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

    UpdateTransMatrix(1);

    BaseMatrix = m_pMeshRenderer->GetMatrix();

    BaseMatrix._41 = 0;
    BaseMatrix._42 = 0;
    BaseMatrix._43 = 0;

    pRigidBody = DefBulletSystem.AddRigidSphere(
        1.5f,
        RigidBody::ct_dynamic,
        m_Params.pos,
        Vector3Zero,
        0.5f,
        0.8f,
        0.25f,
        m_Params.move * 30.0f
        );

    new BallFadeOutRenderer(
        pMesh,
        BaseMatrix,
        pRigidBody,
        60
        );

    m_pStateFunc = &TennisBoundBall::State_Finish;
}

void TennisBoundBall::State_Rolling()
{
    const int RollingFrame = 60;

    if (++m_Timer == RollingFrame)
    {
        m_pStateFunc = &TennisBoundBall::State_AtkStart;
    }
}

void TennisBoundBall::State_AtkStart()
{
    //�G�t�F�N�g
    {
        const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

        //�q�b�g�G�t�F�N�g�쐬
        new HitEffectObject(
            m_Params.pos,
            Vector3Normalize(m_MoveVec),
            0.05f,
            0.15f,
            Vector3(Color.r, Color.g, Color.b)
        );
    }

    //�{�[������
    {
        BallBase::Params param = m_Params;

        //�ړ���
        param.move = Vector3Normalize(m_MoveVec);
        //�X�s�[�h�͓K��
        param.move *= 1.0f;
        //�������L�������ʃ{�[�����˂�Y��(��a�����邩���ȁ`
        param.pos.y = BallBase::UsualBallShotY;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
    }

    m_pStateFunc = &TennisBoundBall::State_Finish;
}