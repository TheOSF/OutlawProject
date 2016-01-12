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
    m_Damage(),
    m_pControllDamage(nullptr)
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
        &m_pAmefootPlayer->m_Renderer.m_TransMatrix
        );

    UpdateDamageTransform();

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
            //�^�b�N���p���[�𗭂߂����Ԃ���v�Z
            m_TacklePower = (float)(m_Timer - kAllFrame) / (float)kAdditionalFrame;
            go_next = true;
        }
    }

    // �ő�t���[���𒴂����玟�̃X�e�[�g��
    if ( m_Timer >= (kAllFrame + kAdditionalFrame) )
    {
        //�^�b�N���p���[�ő�I
        m_TacklePower = 1.0f;
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
    const float kDamagePosOffset = 4.0f;
    const RADIAN kAngleControlSpeed = 0.04f; // �p�x�␳�X�s�[�h

    int kAllFrame = 0;
    float DamageValue = 0.0f;
    float MoveValue = 0.0f;

    //�p�����[�^�擾
    GetTackleParamByPower(m_TacklePower, &kAllFrame, &MoveValue, &DamageValue);


    // �J�n
    if (m_Timer == 0) { TackleStart(MoveValue, DamageValue); }
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
    m_pControllDamage->m_Param.pos = m_pAmefootPlayer->m_Params.pos + chr_func::GetFront(m_pAmefootPlayer) * kDamagePosOffset;
    m_pControllDamage->m_Param.pos.y += kDamagePosYOffset;

    m_pControllDamage->m_Param.size = 2.0f;


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
   // m_pAmefootPlayer->m_Params.move.y = kMoveY;

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
void AmefootAttackClass::TackleStart(float MoveValue, float DamageValue)
{
    const float kAcceleration = MoveValue;

    // �ړ�
    chr_func::ResetMove(m_pAmefootPlayer);
    chr_func::AddMoveFront(m_pAmefootPlayer, kAcceleration, kAcceleration);

    // �_���[�W�ݒ�
    m_Damage.m_Enable = true;
    m_Damage.Value = DamageValue;
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
    m_Damage.m_Enable = false;  //�ʏ�_���[�W��off��

    //�R���g���[���_���[�W�쐬
    {
        class GetTransform :public DamageControllVanish::GetDamageControllTransformClass
        {
            DamageControll_Transform* const m_pTransform;
        public:
            GetTransform(DamageControll_Transform* const pTransform):
                m_pTransform(pTransform)
            {}

            DamageControll_Transform* Get()
            {
                return m_pTransform;
            }
        };

        //����
        m_pControllDamage = new DamageControllVanish(new GetTransform(&m_DamageTransform));

        //�p�����[�^�Z�b�g
        m_pControllDamage->m_Enable = true;
        m_pControllDamage->pParent = m_pAmefootPlayer;
        m_pControllDamage->type = DamageBase::Type::_ControllDamage;
    }


    m_Damage.type = DamageBase::Type::_WeekDamage;
    chr_func::ResetMove(m_pAmefootPlayer);
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// �^�b�`�_�E���J�n�̏u��
void AmefootAttackClass::TouchdownStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Tackle_Touchdown);
}


// �����オ��J�n�̏u��
void AmefootAttackClass::StandupStart()
{
    m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Landing);
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

//�p���[����ːi�̃p�����[�^�𓾂�Q�b�^
void AmefootAttackClass::GetTackleParamByPower(
    RATIO p, int *pOutFrame, float* pOutMoveValue, float* pOutDamagePower)const
{
    struct
    {
        RATIO ok_value;
        int   Frame;
        float MoveValue;
        float Damage;
    }
    Params[]=
    {
        { 1.0f, 32, 1.0f, 6.0f },  //�ő�p���[��
        { 0.9f, 29, 0.9f, 5.0f },
        { 0.7f, 26, 0.8f, 4.0f },
        { 0.5f, 23, 0.7f, 3.0f },
        { 0.3f, 20, 0.6f, 2.0f },
        { 0.1f, 17, 0.5f, 1.0f },  
        { 0.0f, 14, 0.4f, 1.0f }, //�Ŏ�p���[��
    };

    for (auto& it : Params)
    {
        if (it.ok_value <= p)
        {
            *pOutFrame = it.Frame;
            *pOutMoveValue = it.MoveValue;
            *pOutDamagePower = it.Damage;

            return;
        }
    }

    MyAssert(false, "�^�b�N�����߂���������");

}


//�����t���N���X��r�̈ʒu�ɍX�V����
void AmefootAttackClass::UpdateDamageTransform()
{
    Matrix* T = &m_DamageTransform.m_Transform;

    //�r�̍s����擾
    m_pAmefootPlayer->m_Renderer.GetWorldBoneMatirx(*T, 22);

    //�{�[���s��̃X�P�[�������𐳋K��
    for (int i = 0; i < 3; ++i)
    {
        const float Length = Vector3((*T)(0, i), (*T)(1, i), (*T)(2, i)).Length();

        for (int j = 0; j < 3; ++j)
        {
            (*T)(j, i) /= Length;
        }
    }


    //���W�͉E�r�ƍ��r�̒��Ԃ�
    {
        const Vector3 Pos = 
            (m_pAmefootPlayer->m_Renderer.GetLocalBonePos(22)+
             m_pAmefootPlayer->m_Renderer.GetLocalBonePos(28))*0.5f;

        T->_41 = Pos.x;
        T->_42 = Pos.y;
        T->_43 = Pos.z;
    }
}
