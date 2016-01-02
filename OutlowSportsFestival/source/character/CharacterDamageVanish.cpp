#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"


CharacterDamageVanish::Param::Param() :
move(Vector3AxisY),
rotate_speed(Vector3Zero),
down_frame(0),
standup_frame(0)
{

}

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,   //������ԃL�����N�^
    const Param&      param,        //�����Ƃуp�����[�^�[
    Event*            pEvent,       //�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
    DamageManager::HitEventBase* pHitEvent
    ) :
    m_pCharacter(pCharacter),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Initialize),
    m_Count(0),
    m_Rotate(0, 0, 0),
    m_pHitEvent(pHitEvent),
    m_WallHit(false),
    m_FirstSlow(false),
    m_VanishAngle(PI*0.25f),
    m_Locus(15)
{
    m_Param = param;

    {
        m_Locus.m_Visible = false;  //////////

        m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
        m_Locus.m_StartParam.HDRColor = Vector4(0,0,0,0);
        m_Locus.m_StartParam.Width = 1.0f;

        m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0.5f);
        m_Locus.m_EndParam.HDRColor = Vector4(0, 0, 0, 0);
        m_Locus.m_EndParam.Width = 0.1f;
    }
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
    delete m_pHitEvent;
}


// �X�V
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}


void CharacterDamageVanish::Initialize()
{
    //�L�����N�^���_���[�W�̕����Ɍ�������
    chr_func::AngleControll(
        m_pCharacter,
        m_pCharacter->m_Params.pos - m_Param.move
        );

    //�ړ�������ݒ�
    m_pCharacter->m_Params.move = m_Param.move;

    //�X�^�[�g�C�x���g�Ăяo��
    m_pEvent->FlyStart();

    //������уX�e�[�g��
    m_pStateFunc = &CharacterDamageVanish::Flying;

    //�ړ��X�V
    chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());

    //�T�E���h
    Sound::Play(Sound::Damage2);

    
}

void CharacterDamageVanish::Flying()
{
    const float WallRefrectXZmoveValue = 0.2f;

    ++m_Count;

    //�G�t�F�N�g
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0, 2.5f, 0) + Vector3Rand()*0.2f,
            Vector3Zero,
            1.5f + frand()*0.5f, 
            0.1f
            );
    }

    {
        //��s���̉�]�s����쐬����
        Matrix R;

        //{
        //    m_Rotate += m_Param.rotate_speed;

        //    D3DXMatrixRotationYawPitchRoll(
        //        &R,
        //        m_Rotate.y, m_Rotate.x, m_Rotate.z
        //        );
        //}

        {
            float angle = Vector3Radian(m_pCharacter->m_Params.move, -chr_func::GetFront(m_pCharacter));

            if (m_pCharacter->m_Params.move.y < 0)
            {
                angle = -angle;
            }
            else
            {
                angle = 0.0f;
            }

            m_VanishAngle += (angle - m_VanishAngle)*0.1f;

            D3DXMatrixRotationX(&R, m_VanishAngle);
        }
        //������ђ��֐��Ăяo��
        m_pEvent->Flying(R, 1);
    }


    //�n�ʂɂ��Ă����ꍇ�̓X�e�[�g�ڍs
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->DownStart();
        m_pStateFunc = &CharacterDamageVanish::Dowing;
        m_Count = 0;

        //�Y�U�[��
        Sound::Play(Sound::Sand2);
    }

    //�ǂɂ��Ă����ꍇ�͔��]
    if (m_WallHit == false && Vector3XZLength(m_pCharacter->m_Params.move) > WallRefrectXZmoveValue)
    {
        Vector3 out, pos(m_pCharacter->m_Params.pos), vec(m_pCharacter->m_Params.move);
        float dist = 10.0f;
        int material;

        vec.y = 0;
        vec.Normalize();

        pos.y = 3.0f; //���x�͍l�����Ȃ�

        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Character
            ) != nullptr)
        {
            //�ŋߖT�_���Z�o
            Vector3 ToChr = (m_pCharacter->m_Params.pos - out);

            ToChr.y = 0.0f;
            vec.Normalize();

            //�ǂ����苗���ȉ��̏ꍇ
            if (Vector3Dot(ToChr, vec) < m_pCharacter->m_Params.move.Length() + m_pCharacter->m_Params.size)
            {
                m_Count = 0;

                m_pCharacter->m_Params.move = Vector3Refrect(m_pCharacter->m_Params.move, vec)*0.25f;
                chr_func::AngleControll(m_pCharacter, m_pCharacter->m_Params.pos + m_pCharacter->m_Params.move);

                m_pStateFunc = &CharacterDamageVanish::HitWallAndDown;
                m_WallHit = true;

                //�G�t�F�N�g
                DefCamera.SetShock(Vector2(0.3f, 0.3f), 15);

                {
                    Vector3 Pos = out;
                    Pos.y = m_pCharacter->m_Params.pos.y + 3.0f;

                    EffectFactory::CircleAnimation(
                        Pos,
                        vec,
                        Vector3Zero,
                        Vector3Zero,
                        Vector2(8, 8), 
                        0xFFFFFFFF,
                        0x80FFFFFF
                        );
                }
            }
        }
    }

    {
        //�ړ��X�V
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    //�O�ՍX�V
    LocusUpdate(false);
}


void CharacterDamageVanish::Dowing()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->Downing();


    //�G�t�F�N�g
    if (m_Count > 4 && m_Count < 10)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f, 
            2.0f + frand()*2.0f,
            1.0f,
            true
            );
    }

    //�X�e�[�g�ڍs
    if (m_Count > m_Param.down_frame)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUping;
        m_Count = 0;
    }

    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }

    //�O�ՍX�V
    LocusUpdate(true);
}


void CharacterDamageVanish::StandUping()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->StandUping();


    //�N���オ��r���͑��̃A�N�V�����ŃL�����Z���\
    m_pEvent->CanActionUpdate();

    //�X�e�[�g�ڍs
    if (m_Count > m_Param.standup_frame)
    {
        m_pEvent->End();
        m_pStateFunc = &CharacterDamageVanish::End;
        m_Count = 0;
    }

    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }

    //�O�ՍX�V
    LocusUpdate(true);
}


void CharacterDamageVanish::End()
{

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->End();
    }

    //�ړ��X�V
    {
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

}

void CharacterDamageVanish::LocusUpdate(bool FadeOut)
{
    Vector3 Pos, Vec;

    Pos = m_pCharacter->m_Params.pos;
    Pos += Vector3(0, 2, 0);

    Vector3Cross(Vec, m_pCharacter->m_Params.move, DefCamera.GetForward());
    Vec.Normalize();

    m_Locus.AddPoint(Pos, Vec);

    if (FadeOut)
    {
        m_Locus.m_StartParam.Color.w *= 0.9f;
        m_Locus.m_EndParam.Color.w *= 0.9f;
    }
}

void CharacterDamageVanish::HitWallAndDown()
{
    const int DownFrame = 18;

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->HitWall();
    }

    //�G�t�F�N�g
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0, 2.5f, 0) + Vector3Rand()*0.2f,
            Vector3Zero,
            1.5f + frand()*0.5f,
            0.1f
            );
    }

    //�����t���[���Ȃ�
    if (m_Count >= DownFrame)
    {
        //�d�͉��Z
        chr_func::UpdateMoveY(m_pCharacter);

        //���ɓ������Ă����Ȃ�
        if (chr_func::isTouchGround(m_pCharacter))
        {
            m_Count = 0;
            m_pStateFunc = &CharacterDamageVanish::HitFloorAndStandUp;
        }
    }
    else
    {
        ++m_Count;
    }

    //�ʒu���X�V
    chr_func::PositionUpdate(m_pCharacter);

    //�ǂƂ̐ڐG����
    chr_func::CheckWall(m_pCharacter);

    //���Ƃ̐ڐG����
    chr_func::CheckGround(m_pCharacter);

    //�����蔻������
    chr_func::DamageCheck(m_pCharacter, &DamageManager::HitEventBase());


    //�C�x���g�X�V
    m_pEvent->HitWallUpdate();

    //�O�ՍX�V
    LocusUpdate(false);
}



void CharacterDamageVanish::HitFloorAndStandUp()
{
    const int StandUpStart = 10;
    const int StandUpEnd = 50;

    //���ɓ��������C�x���g
    if (m_Count == 0)
    {
        m_pEvent->HitFloor();
    }

    //�G�t�F�N�g
    if (m_Count < StandUpStart)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f,
            2.0f + frand()*2.0f,
            1.0f,
            true
            );
    }
    

    //�����オ��C�x���g
    if (m_Count == StandUpStart)
    {
        m_pEvent->HitFloorAndStandUp();
    }

    //�����オ��I���C�x���g
    if (m_Count == StandUpEnd)
    {
        m_pEvent->End();
    }

    //�J�E���^�[�X�V
    if (m_Count <= StandUpEnd)
    {
        ++m_Count;
    }

    //�N���オ��r���͑��̃A�N�V�����ŃL�����Z���\
    m_pEvent->CanActionUpdate();
    
    //�ړ��X�V
    {
        chr_func::XZMoveDown(m_pCharacter, 0.15f);
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    //�C�x���g�X�V
    m_pEvent->HitWallUpdate();

    //�O�ՍX�V
    LocusUpdate(true);
}