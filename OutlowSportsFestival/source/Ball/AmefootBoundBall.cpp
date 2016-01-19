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
    m_pOwner(pOwner),
    m_ZRotate(0)
{
    //�{�[����{�p�����[�^������
    m_Pos = pos;
    m_Move = first_move;

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
}


AmefootBoundBall::~AmefootBoundBall()
{
    delete m_pBallRenderer;
    m_pBallRenderer = nullptr;
}


bool AmefootBoundBall::Update()
{
    (this->*m_pStateFunc)();

    UpdateMesh();
    UpdateLocusColor();

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
    m_Move.y += Glavity;


    //�y����]
    m_ZRotate += D3DXToRadian(15);

    //�X�e�[�W�Ƃ̔���
    {
        Vector3 out, pos(m_Pos), vec(Vector3Normalize(m_Move));
        float dist = m_Move.Length()*1.5f;
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
                m_Move = Vector3Refrect(m_Move, vec) * 0.6f;
            }
            else
            {
                //����
                SetState(&AmefootBoundBall::StateGroundTouch);
            }

            //���������̈ʒu�ɕ␳����
            m_Pos.y = out.y + 1.0f;
            return;
        }
    }


    //�����Ǘ�(�X�e�[�W�O�ɂ��ʂ����ꍇ�̉��}���u
    if ( ++m_Timer > 300 )
    {
        SetState(&AmefootBoundBall::StateFinish);
    }

    //�ړ��X�V
    m_Pos += m_Move;

    //�O�Ղ̓_��ǉ�
    AddLocusPoint();

}

void AmefootBoundBall::StateGroundTouch()
{
    //�O�Ղ̓_��ǉ�
    AddLocusPoint();

    m_Move = Vector3Zero;

    {

        //��ԋ߂��v���C���[�Ɍ������Ă����悤�ɐݒ�
        const auto& chr_map = DefCharacterMgr.GetCharacterMap();
        float dist = FLT_MAX;
        CharacterBase* pTarget = m_pOwner;

        for ( auto&& chr : chr_map )
        {
            float temp = (chr.first->m_Params.pos - m_Pos).Length();

            if (dist < temp || chr_func::isDie(chr.first))continue;

            dist = temp;
            pTarget = chr.first;
        }

        m_Move = pTarget->m_Params.pos - m_Pos;
        m_Move.y = 0;

        m_Move.Normalize();
        m_Move *= 0.55f;

        //�G�t�F�N�g
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(m_pOwner->m_PlayerInfo.number));

            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                m_Pos,
                Vector3Normalize(m_Move),
                0.1f,
                0.1f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
                );

            //�p�[�e�B�N��
            EffectFactory::Smoke(
                m_Pos,
                Vector3Zero,
                2.0f,
                1.0f,
                true
                );
        }


        //�r�d
        Sound::Play(Sound::Swing2);

        //�ړ��X�e�[�g�Ɉڍs
        SetState(&AmefootBoundBall::StateFinish);


    }

    //�{�[���𐶐��I
    {
        BallBase::Params p;

        p.move = m_Move;
        p.pos = m_Pos;
        p.pParent = m_pOwner;
        p.scale = 0.5f;
        p.type = BallBase::Type::_Usual;
        
        class MoveControll :public UsualBall::MoveControll
        {
        public:
            void Move(UsualBall* pBall)
            {
                pBall->m_Params.pos.y += (UsualBall::UsualBallShotY - pBall->m_Params.pos.y)*0.2f;
                pBall->m_Params.pos += pBall->m_Params.move;
            }
        };

        UsualBall* pBall =  new UsualBall(
            p,
            DamageBase::Type::_WeekDamage,
            2.0f,
            new MoveControll(), 
            1,
            60
            );

        pBall->m_RotateSpeed = Vector3(0, D3DXToRadian(10), 0);
    }
}

void AmefootBoundBall::StateFinish()
{

}

void AmefootBoundBall::SetState(StateFunc pNextState)
{
    m_pStateFunc = pNextState;
    m_Timer = 0;
}

void AmefootBoundBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Pos, v);
}

void AmefootBoundBall::UpdateMesh()
{
    //���b�V���X�V
    const float scale = UsualBall::GetBallScale(CharacterType::_Americanfootball);
    Matrix T, W;

    D3DXMatrixScaling(&T, scale, scale, scale);

    {
        D3DXMatrixRotationY(&W, m_ZRotate);
        T *= W;
    }

    {
        D3DXQUATERNION R;
        D3DXVECTOR3 Axis, v1, v2;
        float Angle;

        v1 = D3DXVECTOR3(0, 1, 0);
        v2 = D3DXVECTOR3(m_Move.x, m_Move.y, m_Move.z);

        D3DXVec3Cross(&Axis, &v1, &v2);

        Angle = D3DXVec3Dot(&v1, &v2) / (D3DXVec3Length(&v1)*D3DXVec3Length(&v2));
        Angle = acosf(fClamp(Angle, 1, -1));

        D3DXQuaternionRotationAxis(&R, &Axis, Angle);

        D3DXMatrixRotationQuaternion(&W, &R);

        T *= W;
    }

    T._41 = m_Pos.x;
    T._42 = m_Pos.y;
    T._43 = m_Pos.z;

    m_pBallRenderer->SetMatrix(T);
}

void AmefootBoundBall::UpdateLocusColor()
{
    //�O�ՐF�X�V
    const COLORf Color = CharacterBase::GetPlayerColorF(m_pOwner->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Color.toVector4();
    m_Locus.m_StartParam.Color.w = 0.3f;
    m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;

    m_Locus.m_StartParam.HDRColor.w = 0.5f;

    m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
    m_Locus.m_EndParam.Color.w = 0;

    m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
    m_Locus.m_EndParam.HDRColor.w = 0;
}

