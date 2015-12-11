#pragma once

#include "../../../Damage/Damage.h"

class AmefootPlayer;

//-----------------------------------------------------------------------------------------
// AmefootThrowBall
// [ �A���t�g ] �������U��(�{�[������)�N���X
// �A���t�g��p�Ȃ̂ŃC�x���g���g�p���Ȃ�
//-----------------------------------------------------------------------------------------
class AmefootThrowBall
{
public:
    struct Params
    {
        int ThrowMotionNumber; // �������[�V�����̔ԍ�

        UINT ThrowFrame; // ������܂ł̃t���[��
        UINT AfterThrowFrame; // �����I�������̃t���[��

        UINT ThrowBoneNumber; // �{�[�����ǂ̃{�[���ɂ��邩

        float ShotPowr; // �ł��o������
        float MoveDownSpeed; // ����
        float Power; // �U����
        float MaxTurnRadian; // �����␳�̊p�x����
        float AngleControlSpeed; // �p�x�ύX���x
    };

public:
    AmefootThrowBall(
        AmefootPlayer* pAmefoot,
        const Params& params,
        DamageManager::HitEventBase* pHitEventBase
        );

    // �X�e�B�b�N�̒l�Z�b�g(��������������߂�)
    void SetStickValue(CrVector2 stick);

    // �X�V
    bool Update();


private:
    void Begin(); // �J�n
    void Throw(); // ����
    void End(); // �I��

private:
    typedef void(AmefootThrowBall::*StateFunc)();

    // �X�e�[�g�ݒ�
    void SetState(StateFunc func);

private:
    DamageManager::HitEventBase* m_pHitEventBase;
    UINT m_Counter; // �J�E���^
    StateFunc m_pStateFunc; // �X�e�[�g�֐�
    AmefootPlayer* m_pAmefoot; // �I�[�i�[�L�����N�^
    const Params m_Params; // �p�����[�^
    Vector2 m_StickValue;

};




