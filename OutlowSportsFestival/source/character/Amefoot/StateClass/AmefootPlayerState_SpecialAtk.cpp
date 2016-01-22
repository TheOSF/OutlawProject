#include "AmefootPlayerState_SpecialAtk.h"

#include "../AmefootUsualHitEvent.h"
#include "../AmefootPlayerState.h"
#include "../../CharacterFunction.h"
#include "../../../Sound/Sound.h"

#include "../../../Effect/SpecialAttackEffect.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"
#include "../../../Collision/Collision.h"

#include "../../../utillity/DamageObject.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Effect/GlavityLocus.h"

#include "../../../Effect/BlurImpact.h"
#include "../../../Effect/ImpactLightObject.h"


const float AmefootPlayerState_SpecialAtk::m_DamageValue = 35.0f;

//--------------------------------------------------//
//  �A���t�g�K�E�Z�X�e�[�g
//--------------------------------------------------//

AmefootPlayerState_SpecialAtk::AmefootPlayerState_SpecialAtk(AmefootPlayer* pCharacter) :
m_pChr(pCharacter)
{
    SetState(&AmefootPlayerState_SpecialAtk::State_Pose);
}

AmefootPlayerState_SpecialAtk::~AmefootPlayerState_SpecialAtk()
{

}


void AmefootPlayerState_SpecialAtk::Enter(AmefootPlayer* pCharacter)
{
    //�_���[�W�g�����X�t�H�[���N���X�̏�����
    m_pDamageTransform = new DamageControll_Transform();
    UpdateDamageTransform();

    class GetDamageTransform :public DamageControllVanish::GetDamageControllTransformClass
    {
        DamageControll_Transform* const pDamageControll_Transform;
    public:
        GetDamageTransform(DamageControll_Transform* const pDamageControll_Transform):
            pDamageControll_Transform(pDamageControll_Transform)
        {
        }

        DamageControll_Transform* Get()
        {
            return pDamageControll_Transform;
        }
    };

    //�_���[�W������
    m_pDamage = new DamageControllVanish(
        new GetDamageTransform(m_pDamageTransform)
        );

    m_pDamage->pParent = m_pChr;
    m_pDamage->m_Param.size = 2.2f;
    m_pDamage->Value = 0.0f;
    m_pDamage->type = DamageBase::Type::_ControllDamage;

    UpdateDamage();

    chr_func::ResetSkillGauge(m_pChr);
}

void AmefootPlayerState_SpecialAtk::Execute(AmefootPlayer* pCharacter)
{
    (this->*m_pStateFunc)();
    
    //�_���[�W�ʒu�X�V
    UpdateDamage();
    UpdateDamageTransform();

    m_pChr->m_Renderer.Update(1);
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}

void AmefootPlayerState_SpecialAtk::Exit(AmefootPlayer* pCharacter)
{
    if (m_pDamageTransform != nullptr)
    {
        m_pDamageTransform->m_Destroy = true;
    }

    delete m_pDamage;
}

//----------------------------PrivateFunc--------------------------------//

void AmefootPlayerState_SpecialAtk::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_Timer = 0;
}

Vector3 AmefootPlayerState_SpecialAtk::GetViewVec()
{
    Vector3 ret(0, 0, 0);

    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //�R���s���[�^����Ȃ�X�e�B�b�N�l��Ԃ�
        Vector2 stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);

        ret = DefCamera.GetForward()*stick.y + DefCamera.GetRight()*stick.x;
    }
    else
    {
        //�R���s���[�^�̏ꍇ�͊p�x�̐󂢓G�Ƀ��b�N
        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(
            m_pChr,
            PI,
            1000.0f,
            &pTarget
            ))
        {
            ret = pTarget->m_Params.pos - m_pChr->m_Params.pos;
        }
    }

    return ret;
}

bool AmefootPlayerState_SpecialAtk::isHitWall(float Dist)
{
    Vector3 out, pos(m_pChr->m_Params.pos + Vector3(0, 2, 0)) , vec;
    float dist = Dist;
    int Mat;

    chr_func::GetFront(m_pChr, &vec);

    return DefCollisionMgr.RayPick(
        &out,
        &pos,
        &vec,
        &dist,
        &Mat,
        CollisionManager::RayType::_Character
        ) != nullptr;

}

void AmefootPlayerState_SpecialAtk::UpdateDamage()
{
    if (m_pDamage == nullptr)
    {
        return;
    }

    //�O�ɂ��炷
    chr_func::GetFront(m_pChr, &m_pDamage->m_Param.pos);
    m_pDamage->m_Param.pos *= 2.0f;

    //�������
    m_pDamage->m_Param.pos += Vector3(0, 3, 0);

    //�L�����N�^�̍��W���Z�b�g
    m_pDamage->m_Param.pos += m_pChr->m_Params.pos;
     
}

void AmefootPlayerState_SpecialAtk::UpdateDamageTransform()
{
    if (m_pDamageTransform == nullptr)
    {
        return;
    }

    Matrix* T = &m_pDamageTransform->m_Transform;
    Matrix W;

    //������
    D3DXMatrixIdentity(T);

    //�c�Ɍ�����(X ���� 90 �x)
    {
        D3DXMatrixRotationX(&W, D3DXToRadian(90));
        *T *= W;
    }

    //�L�����N�^�̌����Ă�������Ɍ�����(�x��)
    {
        D3DXMatrixRotationY(&W, m_pChr->m_Params.angle + PI);
        *T *= W;
    }

    //�A���t�g�̏����O�ɍ��W���Z�b�g
    {
        Vector3 pos;

        chr_func::GetFront(m_pChr, &pos);
        pos *= 5.0f;

        pos += Vector3(0, 1.5f, 0);
        
        pos += m_pChr->m_Params.pos;
        
        T->_41 = pos.x;
        T->_42 = pos.y;
        T->_43 = pos.z;
    }

}

void AmefootPlayerState_SpecialAtk::Explode()
{
    //�t���Ă���L�����N�^�𗣂�
    m_pDamageTransform->AddDamage(m_DamageValue);
    m_pDamageTransform->m_Destroy = true;
    m_pDamageTransform = nullptr;

    //����̃_���[�W�𐶐�
    DamageShpere* pDamage = new DamageShpere();
    
    pDamage->m_Param.pos = m_pChr->m_Params.pos;
    pDamage->m_Param.size = 8.0f;

    pDamage->m_Vec = -m_pChr->m_Params.pos;
    pDamage->m_Vec.y = 0.0f;
    pDamage->m_Vec.Normalize();
    pDamage->m_Vec.y = 0.9f;

    pDamage->m_VecPower = Vector2(0.8f, 0.75f);
    pDamage->m_VecType = DamageShpere::DamageVecType::MemberParam;

    pDamage->pParent = m_pChr;
    pDamage->Value = 0;

    new DamageObject(pDamage, 5);

    //SE
    Sound::Play(Sound::Explode);

    //�G�t�F�N�g
    {
        Vector3  color(1.0f, 0.5f, 0);
        Vector3 power(0, -0.02f, 0);
        Vector3 pos = m_pChr->m_Params.pos + Vector3(0, 2, 0);
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
            move = Vector3Rand() *2.0f;
            move *= frand();
            move.y *= 2.0f;
            move *= 0.75f;

            g = new GlavityLocus(
                pos, move, power, 8, 40 + rand() % 20
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
            SmokePos = pos;

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
            pos + Vector3(0, 2, 0),
            Vector3AxisY,
            Vector3Zero,
            Vector3Zero,
            Vector2(20, 20),
            0x0,
            0xA0FFFFFF,
            1.25f
            );

        //�u���[�G�t�F�N�g
        new BlurImpactSphere(
            pos,
            35,
            80,
            30
            );

        new ImpactLightObject(
            pos + Vector3(0, 3, 0),
            35,
            Vector3(0, 0.5f, 1)*1.0f,
            0.1f
            );


        //�J�����̂��
        DefCamera.SetShock(Vector2(0.5f, 0.5f), 30);
    }
    
}


RADIAN AmefootPlayerState_SpecialAtk::GetControllAngleSpeed()const
{
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        return D3DXToRadian(3);
    }
    else
    {
        switch (m_pChr->m_PlayerInfo.strong_type)
        {
        case StrongType::_Weak:
            return D3DXToRadian(1);

        case StrongType::_Usual:
            return D3DXToRadian(2);

        case StrongType::_Strong:
            return D3DXToRadian(3);

        default:
            break;
        }
    }

    MyAssert(false, "�Ή����Ă��Ȃ������̃^�C�v");

    return 0.0f;
}

void AmefootPlayerState_SpecialAtk::NoDamageUpdate()
{
    class ArmorHitEvent :public DamageManager::HitEventBase
    {
        AmefootPlayer* const  m_pAmefoot;
    public:
        bool isDoArmor;

        ArmorHitEvent(AmefootPlayer* pChr) :
            m_pAmefoot(pChr),
            isDoArmor(false)
        {

        }

        void Effect()
        {
            const Vector3 EffectPos = m_pAmefoot->m_Params.pos + Vector3(0, 2, 0);
            const Vector3 Glavity(0, -0.02f, 0);
            const COLORf  EffectColor(1.0f, 0, 0.8f, 1);

            //�G�t�F�N�g
            new ImpactLightObject(
                EffectPos,
                45,
                Vector3(0, 0.4f, 1),
                0.1f
                );

            //�r�d
            Sound::Play(Sound::AtkHit1);

            for (int i = 0; i < 10; ++i)
            {
                EffectFactory::LocusParticle(
                    EffectPos,
                    (Vector3Rand() - chr_func::GetFront(m_pAmefoot))*1.5f,
                    Glavity,
                    0.1f,
                    8,
                    EffectColor,
                    EffectColor,
                    45,
                    0.1f,
                    0.2f
                    );
            }
        }

        bool Hit(DamageBase* pDmg)
        {
            //�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
            if (pDmg->pParent != nullptr &&
                pDmg->pParent->m_PlayerInfo.number == m_pAmefoot->m_PlayerInfo.number ||
                pDmg->isCanHitCharacter(m_pAmefoot) == false
                )
            {
                return false;
            }

            //pDmg->OnHitCharacter(m_pAmefoot);

            //�G�t�F�N�g
            Effect();

            return true;
        }
    };

    chr_func::UpdateAll(m_pChr, &ArmorHitEvent(m_pChr));
}


// �_���[�W���肪�����Ԃ̍X�V
void AmefootPlayerState_SpecialAtk::UsualDamageUpdate() {

    AmefootUsualHitEvent hitevent(m_pChr);
    chr_func::UpdateAll(m_pChr, &hitevent);

}

//----------------------------StateFunc----------------------------------//


void AmefootPlayerState_SpecialAtk::State_Pose()
{
    const int AllFrame = 55;

    //�J�E���g�i�s
    ++m_Timer;

    //������
    if (m_Timer == 1)
    {
        //�ȑO�̈ړ��l�����Z�b�g
        chr_func::ResetMove(m_pChr);

        //SE
        Sound::Play(Sound::Skill);

        //�����ȊO�̎��Ԃ��Ƃ߂�
        std::list<GameObjectBase*> MoveList;
        MoveList.push_back(m_pChr);
        DefGameObjMgr.FreezeOtherObjectUpdate(MoveList, 55, true);

        //�G�t�F�N�g�N���X����
        new SpecialAttackEffect(m_pChr, 55);

        //���[�V�����Z�b�g
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Start);
    }

    //���Ԍo�߂œːi�X�e�[�g�Ɉڍs
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Dush);
    }

    //�X�V
    NoDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_Dush()
{

    const int       AllFrame = 180;
    const RADIAN    AngleSpeed = GetControllAngleSpeed();
    const float     MoveSpeed = 0.5f;

    //�J�E���g�i�s
    ++m_Timer;

    //������
    if (m_Timer == 1)
    {
        //���[�V�����Z�b�g
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Running);
    }


    //��������
    {
        Vector3 ViewVec = GetViewVec();

        chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + ViewVec, AngleSpeed);
    }

    //���݂̌����Ă�������Ɉړ��l���Z�b�g
    {
        chr_func::GetFront(m_pChr, &m_pChr->m_Params.move);

        m_pChr->m_Params.move *= MoveSpeed;
    }

    //�G������ł�������݃_�b�V���X�e�[�g��
    if (m_pDamage->HitCount > 0)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_HangDush);

        //�X�V
        NoDamageUpdate();

        return;
    }

    //�ǂɓ������Ă����� or ���Ԍo�߂Ŏ��s�X�e�[�g�Ɉڍs
    if (isHitWall(5.0f) || (m_Timer > AllFrame))
    {
        m_pDamage->m_Enable = false;
        m_pDamageTransform->m_Destroy = true;
        m_pDamageTransform =  nullptr;

        SetState(&AmefootPlayerState_SpecialAtk::State_DushEnd);
    }

    //�X�V
    NoDamageUpdate();
}


void AmefootPlayerState_SpecialAtk::State_DushEnd()
{
    const int       AllFrame = 48;
    const float     MoveDownSpeed = 0.15f;

    //�J�E���g�i�s
    ++m_Timer;

    //������
    if (m_Timer == 1)
    {
        //���[�V�����Z�b�g
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Touchdown_Failed);
    }

    //�ړ��ʂ𗎂Ƃ�
    {
        chr_func::XZMoveDown(m_pChr, MoveDownSpeed);
    }

    //���Ԍo�߂ŏI���X�e�[�g��
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Finish);
    }

    //�X�V
    UsualDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_HangDush()
{
    const float MoveSpeed = 0.6f;

    //�J�E���g�i�s
    ++m_Timer;

    //������
    if (m_Timer == 1)
    {
        //���[�V�����Z�b�g
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Running);
    }

    //���݂̌����Ă�������Ɉړ��l���Z�b�g
    {
        chr_func::GetFront(m_pChr, &m_pChr->m_Params.move);

        m_pChr->m_Params.move *= MoveSpeed;
    }

    //�ǂɓ������Ă�����ǂ�����X�e�[�g�Ɉڍs
    if (isHitWall(10.0f))
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_WallHit);
    }

    //�X�V
    NoDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_WallHit()
{
    const int       AllFrame = 48;
    const int       ExplodeFrame = 18;
    const float     MoveDownSpeed = 0.2f;
    const int       AcceFrame = 15;

    //�J�E���g�i�s
    ++m_Timer;

    //������
    if (m_Timer == 1)
    {
        //���[�V�����Z�b�g
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Special_Attack_Finish);
    }

    //����
    if (m_Timer == AcceFrame)
    {
        chr_func::AddMoveFront(m_pChr, 0.6f, 1.0f);
    }


    //�����I
    if (m_Timer == ExplodeFrame)
    {
        Explode();
    }

    //�ړ��ʂ𗎂Ƃ�
    {
        chr_func::XZMoveDown(m_pChr, MoveDownSpeed);
    }

    //���Ԍo�߂ŏI���X�e�[�g��
    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_SpecialAtk::State_Finish);
    }

    //�X�V
    UsualDamageUpdate();
}

void AmefootPlayerState_SpecialAtk::State_Finish()
{
    m_pChr->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pChr));
}