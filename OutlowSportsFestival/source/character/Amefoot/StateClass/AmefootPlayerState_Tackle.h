#pragma once

#include "../AmefootPlayer.h"
#include "../../../Damage/Damage.h"
#include "../../../Effect/SpeedEffect.h"

class AmefootPlayerState_Tackle : public AmefootState
{
public:

    AmefootPlayerState_Tackle(AmefootPlayer* const pCharacter);

    ~AmefootPlayerState_Tackle();


    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

    void StateMachineExit(AmefootPlayer* pCharacter)override;

private:

    AmefootPlayer* const m_pAmefootPlayer;

public:
    typedef void(AmefootPlayerState_Tackle::*StateFunc)();

    // �X�e�[�g�؂�ւ�
    void SetState(StateFunc state);

    void Pose();                // �\����
    void Tackle();              // �^�b�N����
    void DriveAway();           // �^�b�N���ł��グ��
    void Jump();                // ��яオ�蒆
    void Touchdown();           // �^�b�`�_�E����
    void Standup();             // �����オ�蒆
    void Failed();              // ���s
    void End();                 // �I��
    void HitStop();             // �q�b�g�X�g�b�v

    void PoseStart();           // �\���J�n�̏u��
    void TackleStart(float MoveValue); // �^�b�N���J�n�̏u��
    void JumpStart();           // �W�����v�J�n�̏u��
    void DrivaAwayStart();      // �^�b�N���ł��グ�J�n�̏u��
    void TouchdownStart();      // �^�b�`�_�E���J�n�̏u��
    void StandupStart();        // �����オ��J�n�̏u��
    void FailedStart();         // ���s�J�n�̏u��

    void SmokeEffect();

    //�p���[����ːi�̃p�����[�^�𓾂�Q�b�^
    void GetTackleParamByPower(RATIO p, int *pOutFrame, float* pOutMoveValue)const;

    //�����t���N���X��r�̈ʒu�ɍX�V����
    void UpdateDamageTransform();

    //���������������Q�b�g�I
    Vector2 GetControllVec()const;

    //�܂��\���邩�ǂ����𓾂�
    bool isPoseContinue()const;

    //�ʏ�X�V
    void CharacterUsualUpdate();

    //�A�[�}�[��Ԃ̍X�V
    void CharacterNoDamageUpdate();

private:
    DamageControllVanish*      m_pControllDamage;
    DamageControll_Transform*  m_pDamageTransform;
    SpeedEffect                m_SpeedEffect;

    int m_Timer = 0;
    RATIO m_TacklePower = 0.0f;
    StateFunc m_pStateFunc;
};

