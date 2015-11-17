#include "TennisSpecialAtk.h"
#include "../../Ball/UsualBall.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"
#include "TennisPlayerState_UsualMove.h"

TennisSpecialBall::TennisSpecialBall(TennisPlayer* t, CrVector3 pos, CrVector3 move) :
m_pTennis(t),
m_Locus(10),
m_Rotate(0),
m_NoDamageFrame(0)
{
    const float DamageValue = 10.0f;

    {
        //�p�����[�^������
        m_Params.pos = pos;
        m_Params.move = move;
        
        m_Params.pParent = m_pTennis;
        m_Params.scale = 0.5f;
        m_Params.type = BallBase::Type::_Usual;
    }

    {
        //���b�V�������_���쐬
        iexMesh* pMesh;

        UsualBall::GetBallMesh(CharacterType::_Tennis, &pMesh);

        m_pMeshRenderer = new MeshRenderer(
            pMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
            );

        UpdateMesh();
    }


    {
        //�_���[�W������
        m_Damage.pBall = this;
        m_Damage.m_Param.size = 1.5f;
        m_Damage.m_VecPower.x = 0.8f;
        m_Damage.m_VecPower.y = 0.2f;
        m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
        m_Damage.pParent = m_pTennis;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = DamageValue;
    }

    {
        //�O��
        const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

        m_Locus.m_StartParam.Color = Color.toVector4();
        m_Locus.m_StartParam.Color.w = 0.3f;
        m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;

        m_Locus.m_StartParam.HDRColor.w = 0.5f;

        m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
        m_Locus.m_EndParam.Color.w = 0;

        m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
        m_Locus.m_EndParam.HDRColor.w = 0;


        {
            //���b�V�������点��
            COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

            m_pMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;

        }  
    }
}

TennisSpecialBall::~TennisSpecialBall()
{
    delete m_pMeshRenderer;
    m_pMeshRenderer = nullptr;
}

bool TennisSpecialBall::Update()
{
    UpdateMove();
    UpdateDamageClass();
    UpdateMesh();

    return !isOutOfField();
}

bool TennisSpecialBall::Msg(MsgType mt)
{
    return false;
}

void TennisSpecialBall::UpdateDamageClass()
{
    m_Damage.m_Param.pos = m_Params.pos;
    m_Damage.m_Vec = m_Params.move;
}

void TennisSpecialBall::UpdateMesh()
{
    Matrix M;
    const float Scale = UsualBall::GetBallScale(CharacterType::_Tennis);

    D3DXMatrixRotationY(&M, m_Rotate);

    {
        M._11 *= Scale;
        M._12 *= Scale;
        M._13 *= Scale;

        M._21 *= Scale;
        M._22 *= Scale;
        M._23 *= Scale;

        M._31 *= Scale;
        M._32 *= Scale;
        M._33 *= Scale;
    }

    {
        M._41 = m_Params.pos.x;
        M._42 = m_Params.pos.y;
        M._43 = m_Params.pos.z;
    }

    m_pMeshRenderer->SetMatrix(M);
}

void TennisSpecialBall::UpdateMove()
{
    Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
    float dist = m_Params.move.Length();
    int material;

    if (DefCollisionMgr.RayPick(
        &out,
        &pos,
        &vec,
        &dist,
        &material,
        CollisionManager::RayType::_Character
        ) != nullptr)
    {
        vec.y = 0;
        
        m_Params.move = Vector3Refrect(m_Params.move, vec);
    }

    m_Params.pos += m_Params.move;
}


void TennisSpecialBall::Counter(CharacterBase* pCounterCharacter)
{
    m_NoDamageFrame = 5;
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

TennisState_SpecialAtk::TennisState_SpecialAtk(TennisPlayer* t) :
m_pTennis(t),
m_Timer(0)
{

}

TennisState_SpecialAtk::~TennisState_SpecialAtk()
{

}

// �X�e�[�g�J�n
void TennisState_SpecialAtk::Enter(TennisPlayer* t)
{
    //�ȑO�̈ړ��l�����Z�b�g
    chr_func::XZMoveDown(t, 1);
    
    //���[�V�����Z�b�g
    m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_SpecialAtk);

    //�L���C�[���r�d
    Sound::Play(Sound::Skill);

    //�����ȊO�̎��Ԃ��Ƃ߂�
    std::list<GameObjectBase*> MoveList;
    MoveList.push_back(t);
    DefGameObjMgr.FreezeOtherObjectUpdate(MoveList, 55);

}

// �X�e�[�g���s
void TennisState_SpecialAtk::Execute(TennisPlayer* t)
{
    const int ShotFrame = 55;
    const int EndFrame = 85;
    const float BallSpeed = 0.5f;
    //���ԃJ�E���g
    ++m_Timer;

    //����
    if (m_Timer == ShotFrame)
    {
        Vector3 pos, move;

        RADIAN BallAngles[]=
        {
            -PI / 4, 0, PI / 4
        };

        for (int i = 0; i < ARRAYSIZE(BallAngles); ++i)
        {

            pos = m_pTennis->m_Params.pos;
            pos.y = UsualBall::UsualBallShotY;

            chr_func::GetFront(m_pTennis, &move);
            move *= BallSpeed;

            move = Vector3RotateAxis(Vector3AxisY, BallAngles[i], move);

            //�{�[���쐬
            new TennisSpecialBall(
                m_pTennis,
                pos,
                move
                );
        }

        //�r�d
        Sound::Play(Sound::Beam2);
    }


    //��莞�ԂŒʏ�X�e�[�g��
    if (m_Timer > EndFrame)
    {
        m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
    }
   
    
    //��{�I�ȍX�V
    {
        chr_func::UpdateAll(t, &DamageManager::HitEventBase());

        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
        t->m_Renderer.Update(1);
    }
}

// �X�e�[�g�I��
void TennisState_SpecialAtk::Exit(TennisPlayer* t)
{

}