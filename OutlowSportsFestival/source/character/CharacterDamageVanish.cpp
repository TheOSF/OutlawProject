#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"

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
    m_FirstSlow(false)
{
    m_Param = param;
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
    RATIO Speed = 0;

    if (GetKeyState('O'))
    {
        Speed = (m_FirstSlow && m_Count < 30) ? (0.2f) : (1.0f);
    }
    else
    {
        Speed = (m_FirstSlow) ? (min((float)m_Count*0.05f, 1)) : (1.0f);
    }


    ++m_Count;

    //�G�t�F�N�g
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0,2,0) +Vector3Rand()*0.2f, 
            Vector3Zero,
            1.0f + frand()*0.5f,
            0x20FFFFFF
            );
    }

    {
        //��s���̉�]�s����쐬����
        Matrix R;

        m_Rotate += m_Param.rotate_speed;

        D3DXMatrixRotationYawPitchRoll(
            &R,
            m_Rotate.y*Speed, m_Rotate.x*Speed, m_Rotate.z*Speed
            );

        //������ђ��֐��Ăяo��
        m_pEvent->Flying(R, Speed);
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
    if (m_WallHit == false)
    {
        Vector3 out, pos(m_pCharacter->m_Params.pos), vec(m_pCharacter->m_Params.move);
        float dist = m_pCharacter->m_Params.size+ Vector3XZLength(m_pCharacter->m_Params.move)*2.0f;
        int material;

        vec.y = 0;
        vec.Normalize();

        if (DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Usual
            ) != nullptr)
        {
            m_Count = 0;
            vec.y = 0;
            vec.Normalize();

            chr_func::AngleControll(m_pCharacter, m_pCharacter->m_Params.pos);

            m_pCharacter->m_Params.move = Vector3Zero;

            m_pStateFunc = &CharacterDamageVanish::HitWallAndDown;
            m_WallHit = true;

            DefCamera.SetShock(Vector2(0.3f, 0.3f), 15);
        }
    }

    {
        //�ړ��X�V
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase(), Speed);
    }
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
            0xFFFFA080,
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

    //�ړ��X�V
    if (m_Count > m_Param.down_muteki_frame)
    {
        //�ʏ퓖���蔻��
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }
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

    //�ړ��X�V
    if (m_Count > m_Param.standup_muteki_frame)
    {
        //�ʏ퓖���蔻��
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }
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

void CharacterDamageVanish::HitWallAndDown()
{
    const int DownFrame = 18;

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->HitWall();
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
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    //�C�x���g�X�V
    m_pEvent->HitWallUpdate();
}