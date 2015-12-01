#include "TennisSpecialAtk.h"
#include "../../Ball/UsualBall.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/MatchLightManager.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../Effect/SpecialAttackEffect.h"
#include "../../Effect/EffectFactory.h"
#include "../../Effect/GlavityLocus.h"
#include "../../Effect/ImpactLightObject.h"
#include "../../Effect/BlurImpact.h"
#include "../CharacterManager.h"


TennisSpecialBall::TennisSpecialBall(TennisPlayer* t, CrVector3 pos, CrVector3 move) :
m_pTennis(t),
m_Locus(10),
m_Timer(0),
m_Pos(pos),
m_Move(move),
m_pStatefunc(&TennisSpecialBall::StateMove)
{

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
        //�O��
        const COLORf Color = CharacterBase::GetPlayerColorF(t->m_PlayerInfo.number);

        /*m_Locus.m_StartParam.Width = 0.32f;
        m_Locus.m_StartParam.Color = Vector4(0, 0.5f, 1.0f, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 1);

        m_Locus.m_EndParam.Width = 0.0f;
        m_Locus.m_EndParam.Color = Vector4(0, 0.5f, 1.0f, 0);
        m_Locus.m_EndParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 0);

        m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(Color.r, Color.g, Color.b, 1);

        m_Locus.m_EndParam.Width = 0.0f;
        m_Locus.m_EndParam.Color = Vector4(1,1,1,0);
        m_Locus.m_EndParam.HDRColor = Vector4(Color.r, Color.g, Color.b, 0);*/

        m_Locus.m_StartParam.Width = 0.5f;
        m_Locus.m_StartParam.Color = Vector4(0, 0.5f, 1.0f, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 1)*2.0f;

        m_Locus.m_EndParam.Width = 0.0f;
        m_Locus.m_EndParam.Color = Vector4(0, 0.5f, 1.0f, 0);
        m_Locus.m_EndParam.HDRColor = Vector4(1.0f, 1.0f, 1.0f, 0);

        {
            m_pMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
        }  
    }

    {
        m_Damage.Value = 30.0f;

        m_Damage.pParent = m_pTennis;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.m_VecType = DamageShpere::DamageVecType::CenterToPosXZ;

        m_Damage.m_VecPower.x = 0.8f;
        m_Damage.m_VecPower.y = 0.35f;

        m_Damage.m_Enable = false;
    }
}

TennisSpecialBall::~TennisSpecialBall()
{
    delete m_pMeshRenderer;
    m_pMeshRenderer = nullptr;
}

bool TennisSpecialBall::Update()
{

    //BallBase::Params p;
    //p = m_Params;
    //p.type = BallBase::Type::_Usual;

    //UsualBall* dontwork = 
    //new UsualBall(
    //    p,
    //    DamageBase::Type::_WeekDamage,
    //    0.5f
    //    );

    //dontwork->ToNoWork();

    (this->*m_pStatefunc)();



    //�O�Ղ̂Ă���@�ǉ�
    {
        Vector3 v;
        Vector3Cross(v, m_Move, DefCamera.GetForward());
        v.Normalize();

        m_Locus.AddPoint(m_Pos, v);
    }

    return m_pStatefunc != &TennisSpecialBall::StateFinish;
}

bool TennisSpecialBall::Msg(MsgType mt)
{
    return false;
}


//��s���X�e�[�g
void TennisSpecialBall::StateMove()
{
    UpdateMove();
    UpdateMesh();

    //�X�e�[�W�O�ɍs���߂��������
    if (m_Pos.y < -15.0f)
    {
        m_pStatefunc = &TennisSpecialBall::StateFinish;
    }
}

//���e���Ă߂荞�݂Ȃ�?
void TennisSpecialBall::StateStop()
{
    //���b�V��������
    m_pMeshRenderer->m_Visible = false;

    //�O�Ղ̐F�����X�ɂȂ���
    m_Locus.m_StartParam.HDRColor.w *= 0.9f;
    m_Locus.m_StartParam.Color.w *= 0.9f;


    if (m_Timer < 3)
    {
        m_Damage.m_Enable = true;
        m_Damage.m_Param.size = 5.0f;
        m_Damage.m_Param.pos = m_Pos;
    }
    else
    {
        m_Damage.m_Enable = false;
    }

    if (++m_Timer > 10)
    {
        m_pStatefunc = &TennisSpecialBall::StateFinish;
    }

}

//�I��(delete����
void TennisSpecialBall::StateFinish()
{

}



void TennisSpecialBall::UpdateMesh()
{
    Matrix M;
    const float Scale = UsualBall::GetBallScale(CharacterType::_Tennis);

    D3DXMatrixScaling(&M, Scale, Scale, Scale);

    {
        M._41 = m_Pos.x;
        M._42 = m_Pos.y;
        M._43 = m_Pos.z;
    }

    m_pMeshRenderer->SetMatrix(M);
}

void TennisSpecialBall::UpdateMove()
{
    Vector3 out, pos(m_Pos), vec(Vector3Normalize(m_Move));
    float dist = m_Move.Length();
    int material;

    if (DefCollisionMgr.RayPick(
        &out,
        &pos,
        &vec,
        &dist,
        &material,
        CollisionManager::RayType::_Ball
        ) != nullptr)
    {
        //���W�𒅒e�n�_��
        m_Pos = out;

        //�r�d
        Sound::Play(Sound::Explode2);

        //�G�t�F�N�g
        ApperEffect();

        //�X�e�[�g�ڍs
        m_pStatefunc = &TennisSpecialBall::StateStop;
    }

    m_Pos += m_Move;
}

void TennisSpecialBall::ApperEffect()
{
    Vector3  color(1.0f, 0.5f, 0);
    Vector3 Nmove = Vector3Normalize(m_Move);
    Vector3 power(0, -0.02f, 0);
    Vector3 move;
    GlavityLocus* g;

    const Vector4
        stCol(color.x, color.y, color.z, 1.0f),
        endCol(color.x, color.y, color.z, 0.5f);

    const Vector4
        stHdCol(color.x, color.y, color.z, 1.0f),
        endHdCol(color.x, color.y, color.z, 0.5f);


    for (int i = 0; i < 50; ++i)
    {
        move = Vector3Rand() + Nmove;
        move *= frand();
        move.y *= 2.0f;
        move *= 0.75f;

        g = new GlavityLocus(
            m_Pos, move, power, 8, 40 + rand() % 20
            );

        g->m_BoundRatio = 0.5f;
        g->m_CheckWall = false;

        g->m_Locus.m_StartParam.Color = stCol;
        g->m_Locus.m_EndParam.Color = endCol;

        g->m_Locus.m_StartParam.HDRColor = stHdCol;
        g->m_Locus.m_EndParam.HDRColor = endHdCol;

        g->m_Locus.m_StartParam.Width = 0.09f;
        g->m_Locus.m_EndParam.Width = 0.00f;

        // g->m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
    }

    const float SmokeSize = 3.0f;
    Vector3 SmokePos;

    for (int i = 0; i < 20; ++i)
    {
        SmokePos = m_Pos;

        SmokePos.x += (frand() - 0.5f)*5.0f;
        SmokePos.z += (frand() - 0.5f)*5.0f;

        SmokePos.y += frand()*3.5f + SmokeSize*0.5f;

        EffectFactory::Smoke(
            SmokePos,
            Vector3(frand() - 0.5f, 0.5f, frand() - 0.5f)*0.15f, 
            SmokeSize,
            0.2f
            );
    }

    EffectFactory::CircleAnimation(
        m_Pos + Vector3(0, 2, 0), 
        Vector3AxisY,
        Vector3Zero,
        Vector3Zero,
        Vector2(20,20),
        0x0,
        0xA0FFFFFF,
        1.25f
        );

    //�u���[�G�t�F�N�g
    new BlurImpactSphere(
        m_Pos,
        35,
        80,
        30
        );

    new ImpactLightObject(
        m_Pos + Vector3(0, 3, 0), 
        35,
        Vector3(0, 0.5f, 1)*1.0f,
        0.1f
        );


    //�J�����̂��
    DefCamera.SetShock(Vector2(0.5f, 0.5f), 30);
}

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

TennisState_SpecialAtk::TennisState_SpecialAtk(TennisPlayer* t) :
m_pTennis(t),
m_Timer(0),
m_SpeedEffect(1)
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

    //�����ȊO�̎��Ԃ��Ƃ߂�
    std::list<GameObjectBase*> MoveList;
    MoveList.push_back(t);
    DefGameObjMgr.FreezeOtherObjectUpdate(MoveList, 55, true);

    //�G�t�F�N�g
    new SpecialAttackEffect(t, 55);

    //SE
    Sound::Play(Sound::Skill);
}

// �X�e�[�g���s
void TennisState_SpecialAtk::Execute(TennisPlayer* t)
{
    const int ShotFrame = 55;
    const int JumpFrame = 10;
    const int EndFrame = 85;
    const float MoveY = 0.70f;
    const float BallSpeed = 3.0f;

    const RADIAN BallShotAngle = D3DXToRadian(35);

    //���ԃJ�E���g
    ++m_Timer;

    //�W�����v
    if (m_Timer == JumpFrame)
    {
        chr_func::AddMoveFront(m_pTennis, 0.02f, 0.02f);
        m_pTennis->m_Params.move.y = MoveY;

        Vector3 pos = m_pTennis->m_Params.pos + Vector3AxisY*1.5f;

        for (int i = 0; i < 8; ++i)
        {
            EffectFactory::Smoke(
                pos,
                Vector3(frand() - 0.5f, 0, frand() - 0.5f) *0.1f, 
                2.0f,
                0.15f
                );
        }
    }


    if (m_Timer > JumpFrame && m_Timer < JumpFrame + 12)
    {
        m_SpeedEffect.Update(
            m_pTennis->m_Params.pos,
            Vector3AxisY*-0.5f
            );
    }

    //����
    if (m_Timer == ShotFrame)
    {
       
        {
            Vector3 pos, move, auto_move;

            pos = m_pTennis->m_Params.pos;
            pos.y += UsualBall::UsualBallShotY;

            //xz�ɃL�����N�^����x�N�g����
            chr_func::GetFront(m_pTennis, &move);
            move *= BallSpeed;

            //�ړ��x�N�g�������ɉ�]������
            move = Vector3RotateAxis(chr_func::GetRight(m_pTennis), BallShotAngle, move);

            //�␳(������₷���悤��)
            if (CalcToBestTargetVec(pos, move, auto_move))
            {
                move = auto_move;
            }

            //�{�[���쐬
            new TennisSpecialBall(
                m_pTennis,
                pos,
                move
                );
        }

        //�r�d
        Sound::Play(Sound::AtkHit2);
    }


    if (m_Timer > ShotFrame)
    {
        m_pTennis->m_Renderer.m_Lighting *= 0.95f;
    }
    else
    {
        m_pTennis->m_Renderer.m_Lighting += (Vector3(1, 1, 1)*0.2f - m_pTennis->m_Renderer.m_Lighting)*0.2f;
    }

    //��莞�ԂŒʏ�X�e�[�g��
    if (m_Timer > EndFrame)
    {
        m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
    }
   
    
    //��{�I�ȍX�V
    {
        CharacterBase* const p = m_pTennis;

        //�d�͉��Z
        chr_func::UpdateMoveY(p);

        //�ʒu���X�V
        chr_func::PositionUpdate(p);

        //�ǂƂ̐ڐG����
        chr_func::CheckWall(p);

        //���Ƃ̐ڐG����
        chr_func::CheckGround(p);


        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
        t->m_Renderer.Update(1);
    }
}

// �X�e�[�g�I��
void TennisState_SpecialAtk::Exit(TennisPlayer* t)
{
    m_pTennis->m_Renderer.m_Lighting = Vector3Zero;
}

bool TennisState_SpecialAtk::CalcToBestTargetVec(CrVector3 ShotPos, CrVector3 ShotVec, Vector3& out)
{
    //�^�[�Q�b�g�I�聕�����␳
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    RADIAN MostMinAngle = D3DXToRadian(20);   //�����Ƃ������p�x
    RADIAN TempAngle;

    Vector3 toEnemy;

    //�߂�l��������
    bool ret = false;


    for (auto &it : ChrMap)
    {
        //���g�Ǝ���ł���L���������O
        if (m_pTennis->m_PlayerInfo.number == it.first->m_PlayerInfo.number ||
            chr_func::isDie(it.first)
            )
        {
            continue;
        }

        //�G�ւ̃x�N�g��
        toEnemy = (it.first->m_Params.pos - Vector3Normalize(Vector3(ShotVec.x, 0, ShotVec.z))) - ShotPos;

        TempAngle = Vector3Radian(toEnemy, ShotVec);

        //�͈͊O�Ȃ�continue
        if (TempAngle > MostMinAngle)
        {
            continue;
        }

        MostMinAngle = TempAngle;
        out = toEnemy;
        ret = true;
    }

    if (ret)
    {
        out.Normalize();
        out *= ShotVec.Length();
    }

    return ret;
}