#include "AmefootAttackClass.h"
#include "../../CharacterFunction.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../Effect/LocusBarObject.h"
#include "../AmefootPlayer.h"
#include "../AmefootPlayerState.h"
#include "../../CharacterManager.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"



AmefootAttackClass::DamageHitCallback::DamageHitCallback(AmefootAttackClass* pAmefootAttackClass) :
    m_pOwner(pAmefootAttackClass)
{}

//�_���[�W�����������Ƃ��Ɏ����ŌĂ΂��
void AmefootAttackClass::DamageHitCallback::Hit(const SphereParam* sp)
{
    if ( m_pOwner->m_NowStateType == AmefootAttackClass::_Tackle )
    {
        m_pOwner->m_NextStateType = AmefootAttackClass::_DriveAway;
        m_pOwner->m_HitStopFrame = 6;
        m_pOwner->SetState(AmefootAttackClass::_HitStop);
        m_pOwner->m_Damage.m_Enable = false;
        return;
    }

    if ( m_pOwner->m_NowStateType == AmefootAttackClass::_DriveAway )
    {
        m_pOwner->m_NextStateType = AmefootAttackClass::_Jump;
        m_pOwner->m_HitStopFrame = 10;
        m_pOwner->SetState(AmefootAttackClass::_HitStop);
        m_pOwner->m_Damage.m_Enable = false;
        return;
    }
}

//�L�����N�^�ɓ��������Ƃ��ɌĂ΂��
void AmefootAttackClass::DamageHitCallback::HitCharacter(CharacterBase* pHitCharacter)
{
    // ���ĂȂ��I
}



const CharacterBase* GetFrontTargetEnemy(AmefootPlayer* pAmefoot)
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const float  AutoDistance = 10.0f;               //�������ł���ő勗��
    const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

    const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
    RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
    RADIAN TempAngle;

    Vector3 MyFront;      //���g�̑O���x�N�g��
    chr_func::GetFront(pAmefoot, &MyFront);

    auto it = ChrMap.begin();

    while ( it != ChrMap.end() )
    {
        //���g�����O
        if ( pAmefoot->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //���������ȏ�̃L�����N�^�����O����
        if ( Vector3Distance(it->first->m_Params.pos, pAmefoot->m_Params.pos) > AutoDistance )
        {
            it = ChrMap.erase(it);
            continue;
        }

        //�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
        TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - pAmefoot->m_Params.pos));

        //�p�x����ԋ���������X�V
        if ( TempAngle < MostMinAngle )
        {
            pTargetEnemy = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }

    return pTargetEnemy;

}


AmefootAttackClass::AmefootAttackClass(
    AmefootPlayer* const pAmefootPlayer,
    Control* pControl
    ) :
    m_pAmefootPlayer(pAmefootPlayer),
    m_pControl(pControl),
    m_NowStateType(StateType::_Pose),
    m_pNowState(&AmefootAttackClass::Pose),
    m_Damage()
{
    m_Damage.m_Enable = false;
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos;
    m_Damage.m_Param.size = 2.8f;
    m_Damage.pParent = m_pAmefootPlayer;
    m_Damage.pCallBackClass = new DamageHitCallback(this);

}


AmefootAttackClass::~AmefootAttackClass()
{
    delete m_pControl;
}


// �X�V
void AmefootAttackClass::Update()
{
    StateUpdate();

    chr_func::CreateTransMatrix(
        m_pAmefootPlayer,
        m_pAmefootPlayer->m_ModelSize,
        &m_pAmefootPlayer->m_Renderer.m_TransMatrix
        );

    m_pAmefootPlayer->m_Renderer.Update(1);
}


// �X�e�[�g�؂�ւ�
void AmefootAttackClass::SetState(StateType type)
{
    switch ( type )
    {
    case _Pose:m_pNowState = &AmefootAttackClass::Pose; break;
    case _Tackle:m_pNowState = &AmefootAttackClass::Tackle; break;
    case _DriveAway:m_pNowState = &AmefootAttackClass::DriveAway; break;
    case _Jump:m_pNowState = &AmefootAttackClass::Jump; break;
    case _Touchdown:m_pNowState = &AmefootAttackClass::Touchdown; break;
    case _Standup:m_pNowState = &AmefootAttackClass::Standup; break;
    case _TackleFailed:m_pNowState = &AmefootAttackClass::Failed; break;
    case _End:m_pNowState = &AmefootAttackClass::End; break;
    case _HitStop:m_pNowState = &AmefootAttackClass::HitStop; break;
    default:m_pNowState = nullptr; return;
    }
    m_NowStateType = type;
    m_Timer = 0;
    m_Damage.m_Enable = false;
}


// �X�e�[�g�X�V
void AmefootAttackClass::StateUpdate()
{
    if ( m_pNowState != nullptr )
    {
        (this->*m_pNowState)();
    }
}


// �\����
void AmefootAttackClass::Pose()
{
    const int kAllFrame = 15; // �\���t���[��
    const int kAdditionalFrame = 60; // �����ł���t���[��
    const RADIAN kAngleControlSpeed = 0.04f; // �p�x�␳�X�s�[�h
    bool go_next = false; // �X�e�[�g�ڍs���邩�ǂ���
    const float kMoveDownSpeed = 0.2f;

    // �J�n
    if ( m_Timer == 0 ) { PoseStart(); }
    ++m_Timer;

    chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);

    const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy(m_pAmefootPlayer);

    if ( pTargetCharacter != nullptr )
    {
        //������]
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }
    else
    {
        const Vector2 Stick = m_pControl->GetStickValue();

        //�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
        if ( Vector2Length(Stick) > 0.25f )
        {
            Vector3 Vec;

            //�X�e�B�b�N�l���J������Ԃ�
            Vec = DefCamera.GetForward()*Stick.y + DefCamera.GetRight()*Stick.x;

            //�L�����N�^��]
            chr_func::AngleControll(m_pAmefootPlayer, m_pAmefootPlayer->m_Params.pos + Vec, kAngleControlSpeed);
        }
    }


    // �{�^���𗣂����玟�̃X�e�[�g��
    if ( m_Timer >= kAllFrame )
    {
        if ( !m_pControl->isPoseContinue() )
        {
            go_next = true;
        }
    }

    // �ő�t���[���𒴂����玟�̃X�e�[�g��
    if ( m_Timer >= (kAllFrame + kAdditionalFrame) )
    {
        go_next = true;
    }

    // ���̃X�e�[�g��
    if ( go_next )
    {
        SetState(_Tackle);
    }

    // �L�����N�^�[�̍X�V
    CharacterUsualUpdate();
}


// �^�b�N����
void AmefootAttackClass::Tackle()
{
    const int kAllFrame = 15;
    const float kMoveDownSpeed = 0.2f;
    const float kDamagePosOffset = 4.0f;
    const RADIAN kAngleControlSpeed = 0.04f; // �p�x�␳�X�s�[�h

    // �J�n
    if ( m_Timer == 0 ) { TackleStart(); }
    ++m_Timer;

    const CharacterBase * const pTargetCharacter = GetFrontTargetEnemy(m_pAmefootPlayer);
    if ( pTargetCharacter )
    {
        //������]
        chr_func::AngleControll(m_pAmefootPlayer, pTargetCharacter->m_Params.pos, kAngleControlSpeed);
    }

    // �_���[�W���W�ݒ�
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;

    //chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);

    // �t���[���o��
    if ( m_Timer >= kAllFrame )
    {
        SetState(_TackleFailed);
    }

    // �L�����N�^�[�̍X�V
    CharacterNoDamageUpdate();
}


// �^�b�N���ł��グ��
void AmefootAttackClass::DriveAway()
{
    const int kTackleToDriveAwayFrame = 6;
    const int kAllFrame = 15;
    const float kMoveDownSpeed = 0.2f;
    const float kDamagePosOffset = 4.0f;

    // �J�n
    if ( m_Timer == kTackleToDriveAwayFrame )
    {
        DrivaAwayStart();
    }
    else
    {
        chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
    }

    ++m_Timer;

    // �_���[�W���W�ݒ�
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;

    // �t���[���o��
    if ( m_Timer >= kAllFrame )
    {
        SetState(_Jump);
    }

    // �L�����N�^�[�̍X�V
    CharacterNoDamageUpdate();
}


// ��яオ�蒆
void AmefootAttackClass::Jump()
{
    const int kAllFrame = 10;
    const float kMoveSpeed = 0.5f;
    const float kMoveY = 0.8f;
    const float kDamagePosOffset = 4.0f;
    const float kDamagePosYOffset = 2.0f;

    if ( m_Timer == 0 ) { JumpStart(); }
    m_Timer++;

    // �ړ�
    chr_func::AddMoveFront(m_pAmefootPlayer, kMoveSpeed, kMoveSpeed);
    m_pAmefootPlayer->m_Params.move.y = kMoveY;

    // �_���[�W���W�ݒ�
    m_Damage.m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;
    m_Damage.m_Param.pos.y += kDamagePosYOffset;

    if ( m_Timer >= kAllFrame )
    {
        SetState(_Touchdown);
    }

    // �L�����N�^�[�̍X�V
    CharacterNoDamageUpdate();
}


// �^�b�`�_�E����
void AmefootAttackClass::Touchdown()
{
    const int kTouchdownFrame = 35;
    const float kMoveY = -0.8f;
    const float kMoveDownSpeed = 0.5f;

    // �J�n
    if ( m_Timer == 0 ) { TouchdownStart(); }
    ++m_Timer;

    // �ړ�
    chr_func::XZMoveDown(m_pAmefootPlayer, kMoveDownSpeed);
    m_pAmefootPlayer->m_Params.move.y = kMoveY;

    if ( m_Timer >= kTouchdownFrame )
    {
        SetState(_Standup);
    }

    // �L�����N�^�[�̍X�V
    CharacterNoDamageUpdate();
}


// �����オ�蒆
void AmefootAttackClass::Standup()
{
    const int kStandupFrame = 10;

    // �J�n
    if ( m_Timer == 0 ) { StandupStart(); }
    ++m_Timer;

    if ( m_Timer >= kStandupFrame )
    {
        SetState(_End);
    }

    // �L�����N�^�[�̍X�V
    CharacterNoDamageUpdate();
}


// ���s
void AmefootAttackClass::Failed()
{
    const int kFailedFrame = 5;

    // �J�n
    if ( m_Timer == 0 ) { FailedStart(); }
    ++m_Timer;

    if ( m_Timer >= kFailedFrame )
    {
        SetState(_End);
    }

    // �L�����N�^�[�̍X�V
    CharacterUsualUpdate();
}


// �I��
void AmefootAttackClass::End()
{
    m_pAmefootPlayer->SetState(new AmefootPlayerState_UsualMove());
}


// �q�b�g�X�g�b�v
void AmefootAttackClass::HitStop()
{
    assert(m_NextStateType != _Error && "m_NextStateType �� _Error �̏�Ԃ�HitStop�X�e�[�g�����s����܂����B");

    m_Timer++;
    if ( m_Timer >= m_HitStopFrame )
    {
        SetState(m_NextStateType);
        m_NextStateType = _Error;
    }

    // �L�����N�^�[�̍X�V
    CharacterHitStopUpdate();
}


// �\���J�n�̏u��
void AmefootAttackClass::PoseStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Save_Power);
}


// �^�b�N���J�n�̏u��
void AmefootAttackClass::TackleStart()
{
    const float kAcceleration = 0.8f;
    const float kMaxSpeed = 3.0f;

    // �ړ�
    chr_func::AddMoveFront(m_pAmefootPlayer, kAcceleration, kMaxSpeed);

    // �_���[�W�ݒ�
    m_Damage.m_Enable = true;
    m_Damage.Value = 0;
    m_Damage.type = DamageBase::Type::_WeekDamage;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Charge);
}


// �^�b�N���ł��グ�J�n�̏u��
void AmefootAttackClass::DrivaAwayStart()
{
    m_Damage.ResetCounts();
    m_Damage.SetOption(DamageBase::Option::_DontDie, true);
    m_Damage.m_Enable = true;
    m_Damage.m_VecPower = Vector2(0.5f, 0.8f);
    m_Damage.m_Vec = chr_func::GetFront(m_pAmefootPlayer);
    m_Damage.m_Vec.y = 1.0f;
    m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
    m_Damage.Value = 5;
    m_Damage.type = DamageBase::Type::_VanishDamage;

    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Drive_Away);
}


// �W�����v�J�n�̏u��
void AmefootAttackClass::JumpStart()
{
    m_Damage.ResetCounts();
    m_Damage.SetOption(DamageBase::Option::_DontDie, true);
    m_Damage.m_Enable = true;
    m_Damage.m_VecPower = Vector2(0.1f, -1.6f);
    m_Damage.m_Vec = chr_func::GetFront(m_pAmefootPlayer);
    m_Damage.m_Vec.y = 1.0f;
    m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
    m_Damage.Value = 5;
    m_Damage.type = DamageBase::Type::_VanishDamage;

    m_Damage.type = DamageBase::Type::_WeekDamage;
    chr_func::ResetMove(m_pAmefootPlayer);
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// �^�b�`�_�E���J�n�̏u��
void AmefootAttackClass::TouchdownStart()
{
    //m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// �����オ��J�n�̏u��
void AmefootAttackClass::StandupStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Standup);
}


// ���s�J�n�̏u��
void AmefootAttackClass::FailedStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Stand);
}



// �L�����N�^�[�̊�{�I�ȍX�V
void AmefootAttackClass::CharacterUsualUpdate()
{
    AmefootUsualHitEvent HitEvent(m_pAmefootPlayer);
    chr_func::UpdateAll(m_pAmefootPlayer, &HitEvent);
}


// ���G���̍X�V
void AmefootAttackClass::CharacterNoDamageUpdate()
{
    DamageManager::HitEventBase HitEvent;
    chr_func::UpdateAll(m_pAmefootPlayer, &HitEvent);
}


// �q�b�g�X�g�b�v���̍X�V
void AmefootAttackClass::CharacterHitStopUpdate()
{
    DamageManager::HitEventBase HitEvent;

    //�ǂƂ̐ڐG����
    chr_func::CheckWall(m_pAmefootPlayer);

    //���Ƃ̐ڐG����
    chr_func::CheckGround(m_pAmefootPlayer);

    //�����蔻������
    chr_func::DamageCheck(m_pAmefootPlayer, &HitEvent);

}


