#pragma once
#include "../../../Damage/Damage.h"

class AmefootPlayer;

class AmefootAttackClass
{
public:
    class Control
    {
    public:
        // �|�[�Y�𑱂��邩�ǂ���
        virtual bool isPoseContinue() = 0;

        // �X�e�B�b�N�̓��͒l���擾
        virtual Vector2 GetStickValue()const = 0;
    };

    class DamageHitCallback : public DamageBase::HitCallBack
    {
    public:
        DamageHitCallback(AmefootAttackClass* pAmefootAttackClass);

        //�_���[�W�����������Ƃ��Ɏ����ŌĂ΂��
        void Hit(const SphereParam* sp)override;

        //�L�����N�^�ɓ��������Ƃ��ɌĂ΂��
        void HitCharacter(CharacterBase* pHitCharacter)override;

    private:
        AmefootAttackClass* m_pOwner;
    };
    friend DamageHitCallback;


    AmefootAttackClass(
        AmefootPlayer* const pAmefootPlayer,
        Control* pControl
        );


    ~AmefootAttackClass();

    // �X�V
    void Update();


private:
    enum StateType
    {
        _Error,
        _Pose,
        _Tackle,
        _DriveAway,
        _Jump,
        _Touchdown,
        _Standup,
        _TackleFailed,
        _End,
        _HitStop,
    };

    typedef void(AmefootAttackClass::*StateFunc)();

    // �X�e�[�g�؂�ւ�
    void SetState(StateType type);


    // �X�e�[�g�X�V
    void StateUpdate();

    void Pose(); // �\����
    void Tackle(); // �^�b�N����
    void DriveAway(); // �^�b�N���ł��グ��
    void Jump(); // ��яオ�蒆
    void Touchdown(); // �^�b�`�_�E����
    void Standup(); // �����オ�蒆
    void Failed(); // ���s
    void End(); // �I��
    void HitStop(); // �q�b�g�X�g�b�v

    void PoseStart(); // �\���J�n�̏u��
    void TackleStart(float MoveValue, float DamageValue); // �^�b�N���J�n�̏u��
    void JumpStart(); // �W�����v�J�n�̏u��
    void DrivaAwayStart(); // �^�b�N���ł��グ�J�n�̏u��
    void TouchdownStart(); // �^�b�`�_�E���J�n�̏u��
    void StandupStart(); // �����オ��J�n�̏u��
    void FailedStart(); // ���s�J�n�̏u��

    // �L�����N�^�[�̊�{�I�ȍX�V
    void CharacterUsualUpdate();

    // ���G���̍X�V
    void CharacterNoDamageUpdate();

    // �q�b�g�X�g�b�v���̍X�V
    void CharacterHitStopUpdate();

    //�p���[����ːi�̃p�����[�^�𓾂�Q�b�^
    void GetTackleParamByPower(RATIO p, int *pOutFrame, float* pOutMoveValue, float* pOutDamagePower)const;

    //�����t���N���X��r�̈ʒu�ɍX�V����
    void UpdateDamageTransform();

private:
    AmefootPlayer* const m_pAmefootPlayer;
    Control* m_pControl;
    DamageShpere              m_Damage;
    DamageControllVanish*     m_pControllDamage;
    DamageControll_Transform  m_DamageTransform;

    int m_Timer = 0;
    int m_HitStopFrame = 0;
    RATIO m_TacklePower = 0.0f;

    StateType m_NowStateType = _Pose;
    StateType m_NextStateType = _Error;
    StateFunc m_pNowState;

};

