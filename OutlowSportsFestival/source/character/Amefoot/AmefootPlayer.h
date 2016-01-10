#pragma once

#include "../CharacterBase.h"
#include "../CharacterStateTemplate.h"
#include "../CharacterRenderer.h"

class AmefootPlayer;
typedef ChrStateMachine<AmefootPlayer, CharacterBase::MsgType> AmefootStateMachine;
typedef ChrState<AmefootPlayer, CharacterBase::MsgType> AmefootState;


//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ �A���t�g ] �L�����N�^�[�N���X
//-----------------------------------------------------------------------------------------
class AmefootPlayer : public CharacterBase
{
public:
    AmefootPlayer(const PlayerInfo& info);

    ~AmefootPlayer();

    bool Update()override;

    bool CharacterMsg(MsgType msg)override;

    void SetState(AmefootState* state, bool important = false);


public:
    enum MotionType
    {
        Motion_Zero = 0,
        Motion_Stand, // �ҋ@
        Motion_Run_Start, // ����o��
        Motion_Run_End, // ����I���
        Motion_Evasion_Start, // ������Ⴊ��
        Motion_Evasion_Standup, // ����N���オ��
        Motion_Throw_Ball, // �{�[������
        Motion_Tackle_Save_Power, // �^�b�N������
        Motion_Tackle_Charge, // �^�b�N���ːi
        Motion_Tackle_Drive_Away, // �^�b�N���ł��グ
        Motion_Tackle_Touchdown, // �^�b�N���^�b�`�_�E��
        Motion_Tackle_Standup, // �^�b�N���N���オ��
        Motion_Counter_Pose, // �J�E���^�[�\��
        Motion_Counter_Catch_Right, // �J�E���^�[�E�L���b�`
        Motion_Counter_Catch_Left, // �J�E���^�[���L���b�`
        Motion_Counter_Throw_Ball, // �J�E���^�[����
        Motion_Counter_Failed, // �J�E���^�[���s
        Motion_Damage_Weak, // ��U���_���[�W
        Motion_Damage_Vanish_Start, // ������у_���[�W�J�n
        Motion_Damage_Vanish_Landing, // ������у_���[�W���n
        Motion_Damage_Vanish_Standup, // ������у_���[�W�N���オ��
        Motion_Damage_Vanish_Hit_Wall, // ������у_���[�W�ǒ���t��
        Motion_Damage_Vanish_Hit_Wall_Fall, // ������у_���[�W�Ǘ���
        Motion_Damage_Vanish_Hit_Wall_Landing, // ������у_���[�W�ǒn�ʂł͂˂�
        Motion_Damage_Vanish_Hit_Wall_Standup, // ������у_���[�W�ǋN���オ��
        Motion_Touchdown_Start, // �ŗL�Z�J�n
        Motion_Touchdown_Finish, // �ŗL�Z���݂͂���t�B�j�b�V���܂�
        Motion_Touchdown_Failed, // �ŗL�Z���s
        Motion_Touchdown_Failed_Bounced, // �ŗL�Z���s �͂����Ԃ����
        Motion_Special_Attack_Start, // �K�E�Z�J�n
        Motion_Special_Attack_Running, // �K�E�Z����
        Motion_Special_Attack_Finish, // �K�E�Z�t�B�j�b�V��

        Motion_Win_Pose = 0, // �����|�[�Y
        Motion_Lose_Pose = 0, // �����|�[�Y
    };

private:
    AmefootStateMachine* m_pStateMachine;


};
//-----------------------------------------------------------------------------------------

